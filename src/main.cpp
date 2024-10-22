//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <cstdlib>
#include <sys/stat.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "ag_gen/ag_gen.h"
#include "util/db_functions.h"
#include "util/build_sql.h"
#include "util/db.h"
#include "util/hash.h"
#include "util/list.h"
#include "util/mem.h"

#ifdef REDIS
#include "util/redis_manager.h"
#endif // REDIS

template<typename GraphEdge>
class ag_visitor : public boost::default_dfs_visitor {
    std::vector<std::pair<GraphEdge, int>> &to_delete;
  public:
    explicit ag_visitor(std::vector<std::pair<GraphEdge, int>> &_to_delete) : to_delete(_to_delete) {}

    template <typename Graph>
    void back_edge(GraphEdge e, Graph g) {
        typename boost::property_map<Graph, boost::edge_index_t>::type Edge_Index =
                            boost::get(boost::edge_index, g);

        int index = Edge_Index[e];
        // edges[index].set_deleted();
        to_delete.push_back(std::make_pair(e, index));
    }
};

typedef boost::property<boost::edge_name_t, std::string,
    boost::property<boost::edge_index_t, int>> EdgeProperties;
typedef boost::property<boost::vertex_name_t, int> VertexNameProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    VertexNameProperty, EdgeProperties> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor GraphEdge;

Graph graph_init() {
    GraphInfo info = fetch_graph_info();

    auto factbase_ids = info.first;
    auto edges = info.second;

    Graph g;

    boost::property_map<Graph, boost::vertex_name_t>::type Factbase_ID = boost::get(boost::vertex_name, g);
    boost::property_map<Graph, boost::edge_name_t>::type Exploit_ID = boost::get(boost::edge_name, g);
    boost::property_map<Graph, boost::edge_index_t>::type Edge_Index = boost::get(boost::edge_index, g);

    std::unordered_map<int, Vertex> vertex_map;

    for (int fid : factbase_ids) {
        Vertex v = boost::add_vertex(g);
        Factbase_ID[v] = fid;
        vertex_map[fid] = v;
    }

    for (auto ei : edges) {
        int eid = ei[0];
        int from_id = ei[1];
        int to_id = ei[2];
        int exid = ei[3];

        Vertex from_v = vertex_map[from_id];
        Vertex to_v = vertex_map[to_id];

        GraphEdge e = boost::add_edge(from_v, to_v, g).first;
        Exploit_ID[e] = std::to_string(exid);
        Edge_Index[e] = eid;
    }

    return g;
}

void remove_cycles(Graph &g) {
    std::vector<std::pair<GraphEdge, int>> to_delete;

    // ag_visitor<GraphEdge> vis(edges, to_delete);
    ag_visitor<GraphEdge> vis(to_delete);
    boost::depth_first_search(g, boost::visitor(vis));

    std::vector<int> delete_edge_ids;
    delete_edge_ids.resize(to_delete.size());

    for (int i = 0; i < to_delete.size(); ++i) {
        boost::remove_edge(to_delete[i].first, g);
        delete_edge_ids[i] = to_delete[i].second;
    }

    delete_edges(delete_edge_ids);
}

void graph_ag(Graph &g, std::string &filename) {
    std::ofstream gout;
    gout.open(filename);
    boost::write_graphviz(gout, g, boost::default_writer(), boost::make_label_writer(boost::get(boost::edge_name, g)));
}

extern "C" {
    extern FILE *nmin;
    extern int nmparse(networkmodel *nm);
}

std::string parse_nm(std::string &filename) {
    FILE *file = fopen(filename.c_str(), "r");

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
    }

    networkmodel nm;
    nm.assets = list_new();

    //yydebug = 1;
    nmin = file;
    do {
        nmparse(&nm);
    } while(!feof(nmin));

    // FILE *out = stdout;
    std::string output;

    //print_xp_list(xplist);

    /////////////////////////
    // ASSETS
    /////////////////////////

    hashtable *asset_ids = new_hashtable(101);

    // Preload buffer with SQL prelude
    size_t bufsize = INITIALBUFSIZE;
    auto buf = static_cast<char *>(getcmem(bufsize));
    strcat(buf, "INSERT INTO asset VALUES\n");

    // Iterate over each exploit in the list
    // Generate an "exploit_instance" which contains
    // the generated exploit id and the sql for
    // for the exploit.
    for(size_t i=0; i<nm.assets->size; i++) {
        auto asset = static_cast<char *>(list_get_idx(nm.assets, i));
        add_hashtable(asset_ids, asset, i);
        asset_instance *ai = make_asset(asset);
        while(bufsize < strlen(buf) + strlen(ai->sql)) {
            buf = static_cast<char *>(realloc(buf, (bufsize *= 2)));
        }
        strcat(buf, ai->sql);
    }

    // Replace the last comma with a semicolon
    char *last = strrchr(buf, ',');
    *last = ';';
    // fprintf(out, "%s\n", buf);
    output += std::string(buf);

    /////////////////////////
    // FACTS
    /////////////////////////

    // Preload buffer with SQL prelude
    bufsize = INITIALBUFSIZE;
    buf = static_cast<char *>(getcmem(bufsize));
    strcat(buf, "INSERT INTO quality VALUES\n");

    size_t buf2size = INITIALBUFSIZE;
    auto buf2 = static_cast<char *>(getcmem(buf2size));
    strcat(buf2, "INSERT INTO topology VALUES\n");

    // Iterate over each exploit. We then iterate
    // over each f in the exploit and generate
    // the sql for it.
    for(size_t i=0; i<nm.facts->size; i++) {
        auto fct = static_cast<fact *>(list_get_idx(nm.facts, i));
        char *sqlqual,*sqltopo;

        auto assetFrom = static_cast<size_t>(get_hashtable(asset_ids, fct->from));

        switch(fct->type) {
        case QUALITY_T:
            sqlqual = make_quality(assetFrom, fct->st);
            while(bufsize < (strlen(buf) + strlen(sqlqual))) {
                buf = static_cast<char *>(realloc(buf, (bufsize*=2)));
            }
            strcat(buf, sqlqual);
            break;
        case TOPOLOGY_T:
            auto assetTo = static_cast<size_t>(get_hashtable(asset_ids, fct->to));
            sqltopo = make_topology(assetFrom, assetTo, fct->dir, fct->st);
            while(buf2size < (strlen(buf2) + strlen(sqltopo))) {
                buf2 = static_cast<char *>(realloc(buf2, (buf2size*=2)));
            }
            strcat(buf2, sqltopo);
            break;
        }
    }

    last = strrchr(buf, ',');
    *last = ';';

    char *last2 = strrchr(buf2, ',');
    *last2 = ';';

    output += std::string(buf);
    output += std::string(buf2);

    return output;
}

extern "C" {
    extern FILE *xpin;
    extern int xpparse(list *xpplist);
}

std::string parse_xp(std::string &filename) {
    FILE *file = fopen(filename.c_str(), "r");

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
    }

    struct list *xplist = list_new();

    //yydebug = 1;
    xpin = file;
    do {
        xpparse(xplist);
    } while(!feof(xpin));

    // FILE *out = stdout;
    std::string output;

    //print_xp_list(xplist);

    /////////////////////////
    // EXPLOITS
    /////////////////////////

    hashtable *exploit_ids = new_hashtable(101);

    // Preload buffer with SQL prelude
    size_t bufsize = INITIALBUFSIZE;
    auto buf = static_cast<char *>(getcmem(bufsize));
    strcat(buf, "INSERT INTO exploit VALUES\n");

    // Iterate over each exploit in the list
    // Generate an "exploit_instance" which contains
    // the generated exploit id and the sql for
    // for the exploit.
    for(size_t i=0; i<xplist->size; i++) {
        auto xp = static_cast<exploitpattern *>(list_get_idx(xplist, i));
        exploit_instance *ei = make_exploit(xp);
        add_hashtable(exploit_ids, xp->name, ei->id);
        // printf("%s - %d\n", xp->name, get_hashtable(exploit_ids, xp->name));
        while(bufsize < strlen(buf) + strlen(ei->sql)) {
            buf = static_cast<char *>(realloc(buf, (bufsize *= 2)));
        }
        strcat(buf, ei->sql);
    }

    // Replace the last comma with a semicolon
    char *last = strrchr(buf, ',');
    *last = ';';
    // fprintf(out, "%s\n", buf);
    output += std::string(buf);

    /////////////////////////
    // PRECONDITIONS
    /////////////////////////

    // Preload buffer with SQL prelude
    bufsize = INITIALBUFSIZE;
    buf = static_cast<char *>(getcmem(bufsize));
    strcat(buf, "INSERT INTO exploit_precondition VALUES\n");

    // Iterate over each exploit. We then iterate
    // over each f in the exploit and generate
    // the sql for it.
    for(size_t i=0; i<xplist->size; i++) {
        auto xp = static_cast<exploitpattern *>(list_get_idx(xplist, i));
        for(size_t j=0; j<xp->preconditions->size; j++) {
            auto fct = static_cast<fact *>(list_get_idx(xp->preconditions, j));
            // printf("%s: %d\n", fct->from, get_hashtable(exploit_ids, fct->from));
            char *sqladd = make_precondition(exploit_ids, xp, fct);
            while(bufsize < strlen(buf) + strlen(sqladd)) {
                buf = static_cast<char *>(realloc(buf, (bufsize*=2)));
            }
            strcat(buf, sqladd);
        }
    }

    last = strrchr(buf, ',');
    *last = ';';
    // fprintf(out, "%s\n", buf);
    output += std::string(buf);

    /////////////////////////
    // POSTCONDITIONS
    /////////////////////////

    // Preload buffer with SQL prelude
    bufsize = INITIALBUFSIZE;
    buf = (char *)getcmem(bufsize);
    strcat(buf, "INSERT INTO exploit_postcondition VALUES\n");

    // Iterate over each exploit. We then iterate
    // over each f in the exploit and generate
    // the sql for it.
    for(size_t i=0; i<xplist->size; i++) {
        auto xp = static_cast<exploitpattern *>(list_get_idx(xplist, i));
        for(size_t j=0; j<xp->postconditions->size; j++) {
            auto pc = static_cast<postcondition *>(list_get_idx(xp->postconditions, j));
            char *sqladd = make_postcondition(exploit_ids, xp, pc);
            while(bufsize < strlen(buf) + strlen(sqladd)) {
                buf = static_cast<char *>(realloc(buf, (bufsize*=2)));
            }
            strcat(buf, sqladd);
        }
    }

    last = strrchr(buf, ',');
    *last = ';';
    // fprintf(out, "%s\n", buf);
    output += std::string(buf);

    return output;
}

/**
 * @brief      Prints command line usage information.
 */
void print_usage() {
    std::cout << "Usage: ag_gen [OPTION...]" << std::endl << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "\t-c\tConfig section in config.ini" << std::endl;
    std::cout << "\t-b\tEnables batch processing. The argument is the size of batches." << std::endl;
    std::cout << "\t-g\tGenerate visual graph using graphviz, dot file for saving" << std::endl;
    std::cout << "\t-d\tPerform a depth first search to remove cycles" << std::endl;
    std::cout << "\t-n\tNetwork model file used for generation" << std::endl;
    std::cout << "\t-x\tExploit pattern file used for generation" << std::endl;
    std::cout << "\t-r\tUse redis for generation" << std::endl;
    std::cout << "\t-h\tThis help menu." << std::endl;
}

inline bool file_exists(const std::string &name) {
    struct stat buffer {};
    return (stat(name.c_str(), &buffer) == 0);
}

const std::string read_file(const std::string &fn) {
    std::ifstream f(fn);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

// the main function executes the command according to the given flag and throws
// and error if an unknown flag is provided. It then uses the database given in
// the "config.txt" file to generate an attack graph.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    std::string opt_nm;
    std::string opt_xp;
    std::string opt_config;
    std::string opt_graph;
    std::string opt_batch;

    bool should_graph = false;
    bool no_cycles = false;
    bool batch_process = false;
    bool use_redis = false;

    int opt;
    while ((opt = getopt(argc, argv, "rb:g:dhc:n:x:")) != -1) {
        switch (opt) {
        case 'g':
            should_graph = true;
            opt_graph = optarg;
            break;
        case 'h':
            print_usage();
            return 0;
        case 'n':
            opt_nm = optarg;
            break;
        case 'x':
            opt_xp = optarg;
            break;
        case 'c':
            opt_config = optarg;
            break;
        case 'd':
            no_cycles = true;
            break;
        case 'r':
            use_redis = true;
            break;
        case 'b':
            batch_process = true;
            opt_batch = optarg;
            break;
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "wtf\n");
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "Unknown option -%c.\n", optopt);
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
#ifdef REDIS
    if (use_redis) {
        if (!file_exists("redis_scripts/collisions.lua")) {
            fprintf(stderr, "File %s doesn't exist\n", "redis_scripts/collisions.lua");
            exit(EXIT_FAILURE);
        }
    }
#endif // REDIS

    std::string config_section = (opt_config.empty()) ? "default" : opt_config;

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);

    std::string host = pt.get<std::string>(config_section + ".host");
    std::string port = pt.get<std::string>(config_section + ".port");
    std::string dbName = pt.get<std::string>(config_section + ".db");
    std::string username = pt.get<std::string>(config_section + ".username");
    std::string password = pt.get<std::string>(config_section + ".password");

    init_db("postgresql://" + username + ":" + password + "@" + host + ":" + port + "/" +
               dbName);

    std::string parsednm;
    if(!opt_nm.empty()) {
       if (!file_exists(opt_nm)) {
           fprintf(stderr, "File %s doesn't exist.\n", opt_nm.c_str());
           exit(EXIT_FAILURE);
       }
       parsednm = parse_nm(opt_nm);
    }

    std::string parsedxp;
    if(!opt_xp.empty()) {
       if (!file_exists(opt_xp)) {
           fprintf(stderr, "File %s doesn't exist.\n", opt_xp.c_str());
           exit(EXIT_FAILURE);
       }
       parsedxp = parse_xp(opt_xp);
    }

    int batch_size = 0;
    if (batch_process)
       batch_size = std::stoi(opt_batch);

    std::cout << "Importing Models to Database: ";
    import_models(parsednm, parsedxp);
    std::cout << "Done\n";

    AGGenInstance _instance;
    _instance.facts = fetch_facts();
    _instance.initial_qualities = fetch_all_qualities(_instance.facts);
    _instance.initial_topologies = fetch_all_topologies(_instance.facts);
    _instance.assets = fetch_all_assets(_instance.facts);
    _instance.exploits = fetch_all_exploits();
    auto ex = fetch_all_exploits();

    std::cout << "Assets: " << _instance.assets.size() << "\n";
    std::cout << "Exploits: " << _instance.exploits.size() << "\n";
    std::cout << "Facts: " << _instance.facts.size() << "\n";

    AGGenInstance postinstance;
#ifdef REDIS
    if (use_redis) {
        std::cout << "Reading redis scripts\n";

        std::vector<std::pair<std::string, std::string>> sm;
        //sm.push_back(std::make_pair("collisions", read_file("redis_scripts/collisions.lua")));
        sm.emplace_back("collisions", read_file("redis_scripts/collisions.lua"));

        RedisManager rman("127.0.0.1", 6379, sm);
        std::cout << "Done\n";

        std::cout << "Generating Attack Graph: " << std::flush;
        AGGen gen(_instance, rman);
        postinstance = gen.generate(batch_process, batch_size);
    } else {
#endif
        std::cout << "Generating Attack Graph: " << std::flush;
        AGGen gen(_instance);
        postinstance = gen.generate(batch_process, batch_size);
#ifdef REDIS
    }
#endif // REDIS

    std::cout << "Done\n";

    std::cout << "Total Time: " << postinstance.elapsed_seconds.count() << " seconds\n";
    std::cout << "Total States: " << postinstance.factbases.size() << "\n";

    std::cout << "Saving Attack Graph to Database: " << std::flush;
    save_ag_to_db(postinstance, true);
    std::cout << "Done\n";

    if(should_graph) {
        Graph g = graph_init();

        std::cout << "Creating graph visualization: " << std::flush;
        graph_ag(g, opt_graph);
        std::cout << "Done\n";

        if(no_cycles) {
            std::cout << "Removing cycles: " << std::flush;
            remove_cycles(g);
            std::cout << "Done\n";
        }
    }
}

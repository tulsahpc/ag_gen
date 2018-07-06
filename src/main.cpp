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
#include <stdlib.h>
#include <sys/stat.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/depth_first_search.hpp>

// #include <libconfig.h++>

#include "ag_gen/ag_gen.h"
#include "util/db_functions.h"
#include "util/build_sql.h"
#include "util/db.h"
#include "util/hash.h"
#include "util/list.h"
#include "util/mem.h"
#include "util/redis_manager.h"

template<typename GraphEdge>
class ag_visitor : public boost::default_dfs_visitor {
    std::vector<std::pair<GraphEdge, int>> &to_delete;
  public:
    ag_visitor(std::vector<std::pair<GraphEdge, int>> &_to_delete) : to_delete(_to_delete) {}

    template <typename Graph>
    void back_edge(GraphEdge e, Graph g) {
        typename boost::property_map<Graph, boost::edge_index_t>::type Edge_Index =
                            boost::get(boost::edge_index, g);

        int index = Edge_Index[e];
        // edges[index].set_deleted();
        to_delete.push_back(std::make_pair(e, index));
    }
};

void graph_ag(std::string filename, bool should_graph, bool no_cycles) {
    if (!should_graph && !no_cycles) return;

    GraphInfo info = fetch_graph_info();

    auto factbase_ids = info.first;
    auto edges = info.second;

    typedef boost::property<boost::edge_name_t, std::string,
            boost::property<boost::edge_index_t, int>> EdgeProperties;
    typedef boost::property<boost::vertex_name_t, int> VertexNameProperty;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                           VertexNameProperty, EdgeProperties> Graph;

    Graph g;

    boost::property_map<Graph, boost::vertex_name_t>::type Factbase_ID = boost::get(boost::vertex_name, g);
    boost::property_map<Graph, boost::edge_name_t>::type Exploit_ID = boost::get(boost::edge_name, g);
    boost::property_map<Graph, boost::edge_index_t>::type Edge_Index = boost::get(boost::edge_index, g);

    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor GraphEdge;

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

    if (no_cycles) {
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

    if (should_graph) {
        std::ofstream gout;
        gout.open(filename);
        boost::write_graphviz(gout, g, boost::default_writer(), boost::make_label_writer(boost::get(boost::edge_name, g)));
    }
    //boost::write_graphviz(gout, g);

}

extern "C" {
    extern FILE *nmin;
    extern int nmparse(networkmodel *nm);
}

std::string parse_nm(std::string filename) {
    FILE *file = fopen(filename.c_str(), "r");

    networkmodel nm;
    nm.asset_tab = new_hashtable(101);

    nmin = file;
    do {
        nmparse(&nm);
    } while(!feof(nmin));

    str_array* qualities = new_str_array();
    str_array* topologies = new_str_array();

    for(size_t i=0; i<nm.facts->used; i++) {
        char* current = nm.facts->arr[i];
        char* copy = getstr(strlen(current));

        strncpy(copy, current, strlen(current));

        char* type = strsep(&copy, ":");
        if(strncmp(type, "q", 1) == 0) {
            add_str(qualities, copy);
        } else {
            add_str(topologies, copy);
        }
    }

//    FILE *out = stdout;
    std::string output;

    const char* assetheader = "INSERT INTO asset VALUES";
    output += assetheader;

    for(size_t i=0; i<nm.assets->used-1; i++) {
        const char* nextstring = nm.assets->arr[i];
        output += nextstring;
    }
    char* stripped = nm.assets->arr[nm.assets->used-1];
    stripped[strlen(stripped)-1] = '\n';
    output += stripped;
    output += "ON CONFLICT DO NOTHING;";

    const char* qualityheader = "\nINSERT INTO quality VALUES";
    output += qualityheader;
    for(size_t i=0; i<qualities->used-1; i++) {
        const char* nextstring = qualities->arr[i];
        output += nextstring;
    }
    stripped = qualities->arr[qualities->used-1];
    stripped[strlen(stripped)-1] = '\n';
    output += stripped;
    output += "ON CONFLICT DO NOTHING;";

    if(topologies->used > 0) {
        const char *topologyheader = "\nINSERT INTO topology VALUES";
        output += topologyheader;
        for (size_t i = 0; i < topologies->used - 1; i++) {
            const char *nextstring = topologies->arr[i];
            output += nextstring;
        }
        stripped = topologies->arr[topologies->used - 1];
        stripped[strlen(stripped) - 1] = '\n';
        output += stripped;

        output += "ON CONFLICT DO NOTHING;";
    }
    free_hashtable(nm.asset_tab);

    // printf("%s\n", output.c_str());

    return output;
}

extern "C" {
    extern FILE *xpin;
    extern int xpparse(list *xpplist);
}

std::string parse_xp(std::string filename) {
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
    char *buf = static_cast<char *>(getcmem(bufsize));
    strcat(buf, "INSERT INTO exploit VALUES\n");

    // Iterate over each exploit in the list
    // Generate an "exploit_instance" which contains
    // the generated exploit id and the sql for
    // for the exploit.
    for(size_t i=0; i<xplist->size; i++) {
        exploitpattern *xp = static_cast<exploitpattern *>(list_get_idx(xplist, i));
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
        exploitpattern *xp = (exploitpattern *)list_get_idx(xplist, i);
        for(size_t j=0; j<xp->preconditions->size; j++) {
            fact *fct = (fact *)list_get_idx(xp->preconditions, j);
            // printf("%s: %d\n", fct->from, get_hashtable(exploit_ids, fct->from));
            char *sqladd = make_precondition(exploit_ids, xp, fct);
            while(bufsize < strlen(buf) + strlen(sqladd)) {
                buf = (char *)realloc(buf, (bufsize*=2));
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
        exploitpattern *xp = (exploitpattern *)list_get_idx(xplist, i);
        for(size_t j=0; j<xp->postconditions->size; j++) {
            postcondition *pc = (postcondition *)list_get_idx(xp->postconditions, j);
            char *sqladd = make_postcondition(exploit_ids, xp, pc);
            while(bufsize < strlen(buf) + strlen(sqladd)) {
                buf = (char *)realloc(buf, (bufsize*=2));
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
    std::cout << "\t-h\tThis help menu." << std::endl;
}

inline bool file_exists(const std::string &name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

const std::string read_file(const std::string fn) {
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
    std::cout << "yo" << std::endl;

    if (!file_exists("redis_scripts/collisions.lua")) {
        fprintf(stderr, "File %s doesn't exist\n", "redis_scripts/collisions.lua");
        exit(EXIT_FAILURE);
    }

    // cpp_redis::client client;
    // client.connect("127.0.0.1", 6379);
    // client.sadd("helloset", std::vector<std::string>{"420", "69"});
    // client.sync_commit();

    std::string opt_nm;
    std::string opt_xp;
    std::string opt_config;
    std::string opt_graph;
    std::string opt_batch;

    bool should_graph = false;
    bool no_cycles = false;
    bool batch_process = false;

    int opt;
    while ((opt = getopt(argc, argv, "b:g:dhc:n:x:")) != -1) {
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

    std::string config_section = (opt_config.empty()) ? "default" : opt_config;

    // libconfig::Config cfg;

    // try {
    //     cfg.readFile("ag_gen.cfg");
    // } catch (const libconfig::FileIOException &e) {
    //     std::cerr << "Cannot read config file: ./ag_gen.cfg" << std::endl;
    //     exit(1);
    // } catch (const libconfig::ParseException &e) {
    //     std::cerr << "Parse error at " << e.getFile() << ":" << e.getLine() << " - "
    //          << e.getError() << std::endl;
    //     exit(1);
    // }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);

    std::string host = pt.get<std::string>(config_section + ".host");
    std::string port = pt.get<std::string>(config_section + ".port");
    std::string dbName = pt.get<std::string>(config_section + ".db");
    std::string username = pt.get<std::string>(config_section + ".username");
    std::string password = pt.get<std::string>(config_section + ".password");

    // std::string new_db_string;

    // std::string host{"localhost"};
    // std::string port{"5432"};
    // std::string dbName{"ag_gen2"};
    // std::string username{};
    // std::string password{};

    // cfg.lookupValue("database.host", host);
    // cfg.lookupValue("database.port", port);
    // cfg.lookupValue("database.db", dbName);
    // cfg.lookupValue("database.username", username);
    // cfg.lookupValue("database.password", password);

    std::cout << "yo2" << std::endl;

    init_db("postgresql://" + username + "@" + host + ":" + port + "/" +
               dbName);

//    test_create();

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

     std::cout << "Importing Models to Database: " << std::endl;
     import_models(parsednm, parsedxp);
     std::cout << "Done" << std::endl;

     AGGenInstance _instance;
     _instance.facts = fetch_facts();
     _instance.initial_qualities = fetch_all_qualities(_instance.facts);
     _instance.initial_topologies = fetch_all_topologies(_instance.facts);
     _instance.assets = fetch_all_assets(_instance.facts);
     _instance.exploits = fetch_all_exploits();
     auto ex = fetch_all_exploits();

     std::vector<std::pair<std::string, std::string>> sm;
     sm.push_back(std::make_pair("collisions", read_file("redis_scripts/collisions.lua")));

     RedisManager rman("127.0.0.1", 6379, sm);

     AGGen gen(_instance, rman);
     AGGenInstance postinstance = gen.generate(batch_process, batch_size);

     auto factbase_items = postinstance.factbase_items;
     auto factbases = postinstance.factbases;
     auto edges = postinstance.edges;
     auto factlist = postinstance.facts;

     std::cout << "Saving Attack Graph to Database: " << std::endl;
     // save_ag_to_db(factbase_items, factbases, edges, factlist);
     save_ag_to_db(postinstance, true);

     //cleanup
     postinstance = AGGenInstance();

     std::cout << "Done" << std::endl;

     graph_ag(opt_graph, should_graph, no_cycles);
}

//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>

#include <libconfig.h++>

#include "ag_gen/ag_gen.h"
#include "util/db_functions.h"

using namespace std;
using namespace libconfig;

/**
 * @brief      Prints command line usage information.
 */
void print_usage() {
    cout << "Usage: ag_gen [OPTION...]" << endl << endl;
    cout << "Flags:" << endl;
    cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
    cout << "\t-p\tPrint information about the network specified by -n."
         << endl;
    cout << "\t-h\tThis help menu." << endl;
}

void graph_ag(vector<Edge> edges, vector<Factbase> factbases)
{

    typedef boost::property<boost::edge_name_t, string> EdgeNameProperty;
    typedef boost::property<boost::vertex_name_t, int> VertexNameProperty;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                           VertexNameProperty, EdgeNameProperty> Graph;

    Graph g;

    boost::property_map<Graph, boost::vertex_name_t>::type Factbase_ID = boost::get(boost::vertex_name, g);

    boost::property_map<Graph, boost::edge_name_t>::type Exploit_ID = boost::get(boost::edge_name, g);

    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;

    unordered_map<int, Vertex> vertex_map;

    for (auto fbi : factbases)
    {

        Vertex v = boost::add_vertex(g);
        int fid = fbi.get_id();
        Factbase_ID[v] = fid;
        vertex_map[fid] = v;

    }

    for (auto ei : edges)
    {

        int from_id = ei.get_from_id();
        int to_id = ei.get_to_id();
        int eid = ei.get_exploit_id();

        Vertex from_v = vertex_map[from_id];
        Vertex to_v = vertex_map[to_id];

        Edge e = boost::add_edge(from_v, to_v, g).first;
        Exploit_ID[e] = to_string(eid);

    }

    ofstream gout;
    gout.open("ag.circo");
    boost::write_graphviz(gout, g, boost::default_writer(), boost::make_label_writer(boost::get(boost::edge_name, g)));
    //boost::write_graphviz(gout, g);

}

// the main function executes the command according to the given flag and throws
// and error if an unknown flag is provided. It then uses the database given in
// the "config.txt" file to generate an attack graph.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    string opt_nm;
    string opt_xp;

    bool should_graph = false;

    int opt;
    while ((opt = getopt(argc, argv, "hpf:n:g")) != -1) {
        switch (opt) {
        case 'h':
            print_usage();
            return 0;
        case 'n':
            opt_nm = optarg;
            break;
        case 'f':
            opt_xp = optarg;
            break;
        case 'g':
                should_graph = true;
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

    Config cfg;

    try {
        cfg.readFile("ag_gen.cfg");
    } catch (const FileIOException &e) {
        cerr << "Cannot read config file: ./ag_gen.cfg" << endl;
        exit(1);
    } catch (const ParseException &e) {
        cerr << "Parse error at " << e.getFile() << ":" << e.getLine() << " - "
             << e.getError() << endl;
        exit(1);
    }

    string new_db_string;

    string host{"localhost"};
    string port{"5432"};
    string dbName{"ag_gen"};
    string username{};
    string password{};

    cfg.lookupValue("database.host", host);
    cfg.lookupValue("database.port", port);
    cfg.lookupValue("database.db", dbName);
    cfg.lookupValue("database.username", username);
    cfg.lookupValue("database.password", password);

    init_db("postgresql://" + username + "@" + host + ":" + port + "/" +
               dbName);

    AGGenInstance _instance;
    _instance.opt_network = opt_nm;
    _instance.initial_qualities = fetch_all_qualities();
    _instance.initial_topologies = fetch_all_topologies();
    _instance.assets = fetch_all_assets(opt_nm);
    _instance.exploits = fetch_all_exploits();
    _instance.facts = fetch_facts();
    auto ex = fetch_all_exploits();

    AGGen gen(_instance);
    AGGenInstance postinstance = gen.generate();

    auto factbase_items = postinstance.factbase_items;
    auto factbases = postinstance.factbases;
    auto edges = postinstance.edges;
    auto factlist = postinstance.facts;

    save_ag_to_db(factbase_items, factbases, edges, factlist);

    if (should_graph)
        graph_ag(edges, factbases);
}

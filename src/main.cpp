//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

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

    int opt;
    while ((opt = getopt(argc, argv, "hpn:")) != -1) {
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
}

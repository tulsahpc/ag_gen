//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <getopt.h>
#include <iostream>
#include <string>

#include <libconfig.h++>
#include <yaml-cpp/yaml.h>

#include "ag_gen/ag_gen.h"

using namespace std;
using namespace libconfig;

shared_ptr<DB> db;

// print_usage prints to stdout the help menu that corresponds to the ag_gen
// command
void print_usage() {
    cout << "Usage: ag_gen [OPTION...]" << endl << endl;
    cout << "Flags:" << endl;
    cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
    cout << "\t-p\tPrint information about the network specified by -n."
         << endl;
    cout << "\t-h\tThis help menu." << endl;
}

struct Client {
    string Name;
    string OS;
    vector<string> IP;
};

int yaml_test() {
    YAML::Node file = YAML::LoadFile("test.yaml");
    string networkName = file["name"].as<string>();
    Client client1 {};
    YAML::Node client1yaml = file["assets"][0];
    client1.Name = client1yaml["name"].as<string>();
    client1.OS = client1yaml["os"].as<string>();
}

// the main function executes the command according to the given flag and throws
// and error if an unknown flag is provided. It then uses the database given in
// the "config.txt" file to generate an attack graph.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    string opt_network;

    int opt;
    while ((opt = getopt(argc, argv, "hpn:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'n':
                opt_network = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n",
                            optopt);
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
    } catch(const FileIOException &e) {
        cerr << "Cannot read file" << endl;
    } catch(const ParseException &e) {
        cerr << "Parse error at " << e.getFile() << ":" << e.getLine()
             << " - " << e.getError() << endl;
    }

    string new_db_string;

    string host {"localhost"};
    string port {"5432"};
    string dbName {"ag_gen"};
    string username {};
    string password {};

    cfg.lookupValue("database.host", host);
    cfg.lookupValue("database.port", port);
    cfg.lookupValue("database.db", dbName);
    cfg.lookupValue("database.username", username);
    cfg.lookupValue("database.password", password);

    db = make_shared<DB>("postgresql://" + username + "@" + host + ":" + port + "/" + dbName);

    //yaml_test();

    Network net{opt_network};
    AGGen gen{net};

    gen.generate();
}

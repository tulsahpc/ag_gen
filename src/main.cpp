//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <getopt.h>
#include <iostream>
#include <string>

#include <libconfig.h++>

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

vector<Asset> fetch_all_assets(const string net_name)
{

    vector<Row> rows = db->exec("SELECT * FROM asset WHERE network_id = "
                                "(SELECT id FROM network WHERE name = '" +
                                net_name + "');");
    vector<Asset> new_assets;

    for (auto &row : rows) {
        int id = stoi(row[0]);
        string name = row[1];
        int network_id = stoi(row[2]);

        new_assets.emplace_back(id, network_id, name);
    }

    return new_assets;

}

vector<string> fetch_quality_attributes()
{

    vector<string> attrs;
    vector<Row> qrows = db->exec("SELECT DISTINCT property FROM quality;");
    vector<Row> erows =
        db->exec("SELECT DISTINCT property FROM exploit_postcondition;");

    for (auto &row : qrows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    for (auto &row : erows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    return attrs;

}

vector<string> fetch_quality_values()
{

    vector<string> vals;
    vector<Row> qrows = db->exec("SELECT DISTINCT value FROM quality;");
    vector<Row> erows =
        db->exec("SELECT DISTINCT value FROM exploit_postcondition;");

    for (auto &row : qrows) {
        string val = row[0];
        vals.push_back(val);
    }

    for (auto &row : erows) {
        string val = row[0];
        vals.push_back(val);
    }

    return vals;

}

vector<string> fetch_topology_attributes()
{

    vector<string> attrs;
    vector<Row> rows = db->exec("SELECT DISTINCT property FROM topology;");

    for (auto &row : rows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    return attrs;

}

vector<string> fetch_topology_values()
{

    vector<string> vals;
    vector<Row> rows = db->exec("SELECT DISTINCT value FROM topology;");

    for (auto &row : rows) {
        string val = row[0];
        vals.push_back(val);
    }

    return vals;

}

Keyvalue fetch_facts()
{

    Keyvalue initfacts;

    initfacts.populate(fetch_quality_attributes());
    initfacts.populate(fetch_quality_values());
    initfacts.populate(fetch_topology_attributes());
    initfacts.populate(fetch_topology_values());

    return initfacts;

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

    Network net{opt_network};

    //Network net{fetch_all_assets(opt_network), fetch_facts()};
    AGGen gen{net};

   gen.generate();
}

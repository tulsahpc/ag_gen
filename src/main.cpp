//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <getopt.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>

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

/**
 * @brief Fetches the preconditions of an Exploit from the database.
 */
unordered_map<int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>> fetch_exploit_preconds() {
    vector<Row> rows =
        db->exec("SELECT * FROM exploit_precondition");

    unordered_map<int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>> precond_map;
    
    int curr_id = -1;
    vector<ParameterizedQuality> preconds_q;
    vector<ParameterizedTopology> preconds_t;
    for (auto &row : rows) {
        int type = stoi(row[2]);
        int exploit_id = stoi(row[1]);
        cout << "ID: " << exploit_id << endl;
        if (exploit_id != curr_id)
        {

            if (curr_id != -1)
            {
                tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{preconds_q, preconds_t};
                precond_map[curr_id] = tup;

                preconds_q.clear();
                preconds_t.clear();
                // vector<ParameterizedQuality> preconds_q;
                // vector<ParameterizedTopology> preconds_t;

            }

            curr_id = exploit_id;

        }

        if (type == 0) {
            int param1 = stoi(row[3]);
            string property = row[5];
            string value = row[6];

            ParameterizedQuality qual{param1, property, value};
            preconds_q.push_back(qual);
        } else {
            int param1 = stoi(row[3]);
            int param2 = stoi(row[4]);
            string property = row[5];
            string value = row[6];
            string op = row[7];
            string dir = row[8];

            ParameterizedTopology topo(param1, param2, dir, property, op,
                                       value);
            preconds_t.push_back(topo);
        }
    }

    tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{preconds_q, preconds_t};
    precond_map[curr_id] = tup;

    return precond_map;
}

/**
 * @brief Fetches the postconditions of an Exploit from the database.
 */
unordered_map<int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>> fetch_exploit_postconds() {
    vector<Row> rows =
        db->exec("SELECT * FROM exploit_postcondition");

    cout << "LENGTH: " << rows.size() << endl;
    unordered_map<int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>> postcond_map;

    int curr_id = -1;
    vector<ParameterizedQuality> postconds_q;
    vector<ParameterizedTopology> postconds_t;
    for (auto &row : rows) {
        int type = stoi(row[2]);
        int exploit_id = stoi(row[1]);

        if (exploit_id != curr_id)
        {

            if (curr_id != -1)
            {

                tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{postconds_q, postconds_t};
                postcond_map[curr_id] = tup;

                postconds_q.clear();
                postconds_t.clear();
                // vector<ParameterizedQuality> postconds_q;
                // vector<ParameterizedTopology> postconds_t;

            }

            curr_id = exploit_id;

        }

        if (type == 0) {
            int param1 = stoi(row[3]);
            string property = row[5];
            string value = row[6];

            ParameterizedQuality qual{param1, property, value};
            postconds_q.push_back(qual);
        } else {
            int param1 = stoi(row[3]);
            int param2 = stoi(row[4]);
            string property = row[5];
            string value = row[6];
            string op = row[7];
            string dir = row[8];

            ParameterizedTopology topo(param1, param2, dir, property, op,
                                       value);
            postconds_t.push_back(topo);
        }
    }
   
    tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{postconds_q, postconds_t};
    postcond_map[curr_id] = tup;

    return postcond_map;
    
}

vector<Exploit> fetch_all_exploits() {
    vector<Exploit> exploits;
    vector<Row> rows = db->exec("SELECT * FROM exploit;");

    auto pre = fetch_exploit_preconds();
    auto post = fetch_exploit_postconds();

    for (auto &row : rows) {
        int id = stoi(row[0]);
        string name = row[1];
        int num_params = stoi(row[2]);

        auto preconds = pre[id];
        auto postconds = post[id];

        Exploit exploit(id, name, num_params, preconds, postconds);
        exploits.push_back(exploit);
    }

    return exploits;
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

AGGenInstance build_pre_instance(const string net_name)
{

    
    Keyvalue facts = fetch_facts();    

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

    /*
    auto m = fetch_exploit_preconds();

    cout << endl << endl << "Preconds" << endl;
    for (auto mi : m)
    {

        cout << "exploit_id: " << mi.first << endl;
        cout << "Qualities" << endl;
        vector<ParameterizedQuality> mq = get<0>(mi.second);
        for (auto mqi : mq)
            mqi.print();

        cout << "Topologies" << endl;
        vector<ParameterizedTopology> mt = get<1>(mi.second);
        for (auto mti : mt)
            mti.print();

    }

    auto m2 = fetch_exploit_postconds();

    cout << endl << endl << "Postconds" << endl;
    for (auto mi : m2)
    {

        cout << "exploit_id: " << mi.first << endl;
        cout << "Qualities" << endl;
        vector<ParameterizedQuality> mq = get<0>(mi.second);
        for (auto mqi : mq)
            mqi.print();

        cout << "Topologies" << endl;
        vector<ParameterizedTopology> mt = get<1>(mi.second);
        for (auto mti : mt)
            mti.print();

    }*/

    //Network net{fetch_all_assets(opt_network), fetch_facts()};
    auto ex = fetch_all_exploits();
    AGGen gen{net};
    //AGGenInstance preinst = build_pre_instance(opt_network);
    //AGGen gen{preinst};

   gen.generate(ex);
}

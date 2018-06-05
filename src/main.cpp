//! main.cpp contains the main fuction that runs the program including flag
//! handling and calls to functions that access the database and generate the
//! attack graph.
//!

#include <getopt.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <stdlib.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>

#include <libconfig.h++>

#include "ag_gen/ag_gen.h"
#include "util/db.h"

using namespace std;
using namespace libconfig;

//std::shared_ptr<DB> db;
DB db;

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

vector<string> fetch_quality_attributes()
{

    vector<string> attrs;
    vector<Row> qrows = db.exec("SELECT DISTINCT property FROM quality;");
    vector<Row> erows =
        db.exec("SELECT DISTINCT property FROM exploit_postcondition;");

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
    vector<Row> qrows = db.exec("SELECT DISTINCT value FROM quality;");
    vector<Row> erows =
        db.exec("SELECT DISTINCT value FROM exploit_postcondition;");

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
    vector<Row> rows = db.exec("SELECT DISTINCT property FROM topology;");

    for (auto &row : rows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    return attrs;

}

vector<string> fetch_topology_values()
{

    vector<string> vals;
    vector<Row> rows = db.exec("SELECT DISTINCT value FROM topology;");

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
        db.exec("SELECT * FROM exploit_precondition");

    unordered_map<int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>> precond_map;
    
    int curr_id = -1;
    vector<ParameterizedQuality> preconds_q;
    vector<ParameterizedTopology> preconds_t;
    for (auto &row : rows) {
        int type = stoi(row[2]);
        int exploit_id = stoi(row[1]);
        // cout << "ID: " << exploit_id << endl;
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
        db.exec("SELECT * FROM exploit_postcondition");

    // cout << "LENGTH: " << rows.size() << endl;
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
    vector<Row> rows = db.exec("SELECT * FROM exploit;");

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

/**
 * @brief Gets all of the qualities for the Asset
 * @details Grabs all of the qualities in the database associated with
 *          the Asset's ID and gives them to the Asset
 */
unordered_map<int, vector<Quality>> fetch_asset_qualities() {
    vector<Row> rows = db.exec("SELECT * FROM quality");

    unordered_map<int, vector<Quality>> qmap;

    int curr_id = -1;
    vector<Quality> qualities;
    for (auto &row : rows) {
        int asset_id = stoi(row[0]);
        string property = row[1];
        string op = row[2];
        string value = row[3];
        // cout << "op: " << op << endl;

        if (asset_id != curr_id)
        {

            if (curr_id != -1)
            {

                qmap[curr_id] = qualities;
                qualities.clear();

            }

            curr_id = asset_id;

        }
        // Quality qual(asset_id, property, "=", value);
        // qualities.push_back(qual);

        qualities.emplace_back(asset_id, property, op, value);
    }

    qmap[curr_id] = qualities;

    return qmap;
}

/**
 * @brief Gets all of the Assets under the network
 * @details Grabs all of the Assets in the database under the network given in
 *          the argument and returns a vector of those Assets
 *
 * @param network Name of the network to grab from
 */
vector<Asset> fetch_all_assets(const string &network) {
    vector<Row> rows = db.exec("SELECT * FROM asset WHERE network_id = "
                                "(SELECT id FROM network WHERE name = '" +
                                network + "');");
    vector<Asset> new_assets;

    auto qmap = fetch_asset_qualities();

    for (auto &row : rows) {
        int id = stoi(row[0]);
        string name = row[1];
        int network_id = stoi(row[2]);

        auto q = qmap[id];

        // new_assets.push_back(asset);
        new_assets.emplace_back(id, network_id, name, q);
    }

    return new_assets;
}

vector<Quality> fetch_all_qualities() {
    vector<Quality> qualities;
    vector<Row> rows = db.exec("SELECT * FROM quality;");

    for (auto &row : rows) {
        int asset_id = stoi(row[0]);
        string property = row[1];
        string op = row[2];
        string value = row[3];

        Quality qual(asset_id, property, op, value);
        qualities.push_back(qual);
    }

    return qualities;
}

vector<Topology> fetch_all_topologies() {
    vector<Topology> topologies;

    vector<Row> rows = db.exec("SELECT * FROM topology;");
    for (auto &row : rows) {
        int from_asset = stoi(row[0]);
        int to_asset = stoi(row[1]);
        string dir = row[2];
        string property = row[3];
        string op = row[4];
        string value = row[5];

        Topology t(from_asset, to_asset, dir, property, op, value);
        topologies.push_back(t);
    }

    return topologies;
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

inline string to_query(Edge edge){ return edge.get_query(); }

void save_ag_to_db(std::vector<FactbaseItems> &factbase_items, std::vector<Factbase> &factbases, 
                   std::vector<Edge> &edges, Keyvalue &factlist)
{

    string factbase_sql_query = "INSERT INTO factbase VALUES ";

    for (int i = 0; i < factbases.size(); ++i)
    {

        if (i == 0)
            factbase_sql_query += "(" + to_string(factbases[i].get_id()) + ",'" + to_string(factbases[i].hash(factlist)) + "')";

        else
            factbase_sql_query += ",(" + to_string(factbases[i].get_id()) + ",'" + to_string(factbases[i].hash(factlist)) + "')";

    }

    factbase_sql_query += " ON CONFLICT DO NOTHING;";

    // cout << factbase_sql_query << endl;

    db.exec(factbase_sql_query);

    string item_sql_query = "INSERT INTO factbase_item VALUES ";

    string quality_sql_query = "";
    string topology_sql_query = "";

    for (int j = 0; j < factbase_items.size(); ++j)
    {

        auto fbi = factbase_items[j];

        int id = get<1>(fbi);
        auto items = get<0>(fbi);

        auto quals = get<0>(items);
        auto topo = get<1>(items);

        for (auto qi : quals)
        {

            if (j == 0)
                quality_sql_query += "(" + to_string(id) + "," + to_string(qi.encode(factlist).enc) + ",'quality')";

            else
                quality_sql_query += ",(" + to_string(id) + "," + to_string(qi.encode(factlist).enc) + ",'quality')";

        }

        for (auto ti : topo)
        {

            if (j == 0)
                topology_sql_query += "(" + to_string(id) + "," + to_string(ti.encode(factlist).enc) + ",'topology')";

            else
                topology_sql_query += ",(" + to_string(id) + "," + to_string(ti.encode(factlist).enc) + ",'topology')";

        }

    }

    if (topology_sql_query != "")
        item_sql_query += quality_sql_query + "," + topology_sql_query + " ON CONFLICT DO NOTHING;";
    else
        item_sql_query += quality_sql_query + " ON CONFLICT DO NOTHING;";

    db.exec(item_sql_query);

    string edge_sql_query = "INSERT INTO edge VALUES ";
    string edge_assets_sql_query = "INSERT INTO edge_asset_binding VALUES ";

    vector<string> edge_queries;
    edge_queries.resize(edges.size());

    transform(edges.begin(), edges.end(), edge_queries.begin(), to_query);

    // map edge queries to index in edge vector
    unordered_map<string, int> eq;
    auto ei = edge_queries.begin();
    for (int i = 0; ei != edge_queries.end(); i++, ei++)
        eq.insert({*ei, i});

    int ii = 0;
    for (auto ei : eq)
    {

        int i = ei.second;

        int eid = edges[i].set_id();

        if (ii == 0)
        {
            edge_sql_query += "(" + to_string(eid) + "," + ei.first;
            edge_assets_sql_query += edges[i].get_asset_query();
        }
        else
        {
            edge_sql_query += ",(" + to_string(eid) + "," + ei.first;
            edge_assets_sql_query += "," + edges[i].get_asset_query();
        }

        ++ii;

    }

    edge_sql_query += " ON CONFLICT DO NOTHING;";
    edge_assets_sql_query += " ON CONFLICT DO NOTHING;";

    // cout << edge_sql_query << endl << endl;
    // cout << edge_assets_sql_query << endl << endl;

    db.exec(edge_sql_query);
    db.exec(edge_assets_sql_query);

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

    string opt_network;

    bool should_graph = false;

    int opt;
    while ((opt = getopt(argc, argv, "hpn:g")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'n':
                opt_network = optarg;
                break;
            case 'g':
                should_graph = true;
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

    // db = make_shared<DB>("postgresql://" + username + "@" + host + ":" + port + "/" + dbName);
    db.connect("postgresql://" + username + "@" + host + ":" + port + "/" + dbName);
    // db = DB {"postgresql://" + username + "@" + host + ":" + port + "/" + dbName};

    //db.connect("postgresql://" + username + "@" + host + ":" + port + "/" + dbName);

    /*
    auto a = fetch_asset_qualities();

    for (auto ai : a)
    {

        cout << "Asset ID: " << ai.first << endl;
        for (auto aiq : ai.second)
        {

            aiq.print();

        }

    }
    */

    AGGenInstance _instance;

    _instance.opt_network = opt_network;
    _instance.initial_qualities = fetch_all_qualities();
    _instance.initial_topologies = fetch_all_topologies();
    _instance.assets = fetch_all_assets(opt_network);
    _instance.exploits = fetch_all_exploits();
    _instance.facts = fetch_facts();

    // Network net{opt_network, fetch_all_qualities(), fetch_all_topologies(), fetch_all_assets(opt_network), fetch_facts()};

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
    AGGen gen(_instance);
    //AGGenInstance preinst = build_pre_instance(opt_network);
    //AGGen gen{preinst};

    AGGenInstance postinstance = gen.generate();

    auto factbase_items = postinstance.factbase_items;
    auto factbases = postinstance.factbases;
    auto edges = postinstance.edges;
    auto factlist = postinstance.facts;

    save_ag_to_db(factbase_items, factbases, edges, factlist);

    if (should_graph)
        graph_ag(edges, factbases);


}

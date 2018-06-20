
#include <vector>
#include <string>
#include <unordered_map>

#include "db_functions.h"

#include "keyvalue.h"
#include "db.h"

#include "ag_gen/parameterized_quality.h"
#include "ag_gen/parameterized_topology.h"
#include "ag_gen/exploit.h"
#include "ag_gen/edge.h"
#include "ag_gen/quality.h"
#include "ag_gen/topology.h"
#include "ag_gen/asset.h"
#include "ag_gen/factbase.h"

using namespace std;

static DB db;

void init_db(std::string connect_str) {
    db.connect(connect_str);
}

void import_models(std::string nm, std::string xp) {
    db.exec(nm);
    db.exec(xp);
}

/**
 * @brief       Fetches all possible quality attributes.
 *
 * @return     Returns a vector of strings with all possible quality attributes.
 */
vector<string> fetch_quality_attributes() {
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

/**
 * @brief      Fetches all possible quality values.
 *
 * @return     Returns
 */
vector<string> fetch_quality_values() {
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

vector<string> fetch_topology_attributes() {
    vector<string> attrs;
    vector<Row> rows = db.exec("SELECT DISTINCT property FROM topology;");

    for (auto &row : rows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    return attrs;
}

/**
 * @brief      Fetches all possible topology values.
 *
 * @return     Returns a vector of strings with all possible topology values
 */
vector<string> fetch_topology_values() {
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
unordered_map<
    int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>>
fetch_exploit_preconds() {
    vector<Row> rows = db.exec("SELECT * FROM exploit_precondition;");

    unordered_map<
        int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>>
        precond_map;

    int curr_id = -1;
    vector<ParameterizedQuality> preconds_q;
    vector<ParameterizedTopology> preconds_t;
    for (auto &row : rows) {
        int type = stoi(row[2]);
        int exploit_id = stoi(row[1]);

        if (exploit_id != curr_id) {
            if (curr_id != -1) {
                tuple<vector<ParameterizedQuality>,
                      vector<ParameterizedTopology>>
                    tup{preconds_q, preconds_t};
                precond_map[curr_id] = tup;

                preconds_q.clear();
                preconds_t.clear();
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

            ParameterizedTopology topo{param1,   param2, dir,
                                       property, op,     value};
            preconds_t.push_back(topo);
        }
    }

    tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{
        preconds_q, preconds_t};
    precond_map[curr_id] = tup;

    return precond_map;
}

/**
 * @brief Fetches the postconditions of an Exploit from the database.
 */
unordered_map<
    int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>>
fetch_exploit_postconds() {
    vector<Row> rows = db.exec("SELECT * FROM exploit_postcondition;");

    unordered_map<
        int, tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>>>
        postcond_map;

    int curr_id = -1;
    vector<ParameterizedQuality> postconds_q;
    vector<ParameterizedTopology> postconds_t;
    for (auto &row : rows) {
        int type = stoi(row[2]);
        int exploit_id = stoi(row[1]);

        if (exploit_id != curr_id) {
            if (curr_id != -1) {
                tuple<vector<ParameterizedQuality>,
                      vector<ParameterizedTopology>>
                    tup{postconds_q, postconds_t};
                postcond_map[curr_id] = tup;

                postconds_q.clear();
                postconds_t.clear();
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

            ParameterizedTopology topo{param1,   param2, dir,
                                       property, op,     value};
            postconds_t.push_back(topo);
        }
    }

    tuple<vector<ParameterizedQuality>, vector<ParameterizedTopology>> tup{
        postconds_q, postconds_t};
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
    vector<Row> rows = db.exec("SELECT * FROM quality;");

    unordered_map<int, vector<Quality>> qmap;

    int curr_id = -1;
    vector<Quality> qualities;
    for (auto &row : rows) {
        int asset_id = stoi(row[0]);
        string property = row[1];
        string op = row[2];
        string value = row[3];
        // cout << "op: " << op << endl;

        if (asset_id != curr_id) {
            if (curr_id != -1) {
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
vector<Asset> fetch_all_assets() {
    vector<Row> rows = db.exec("SELECT * FROM asset;");
    vector<Asset> new_assets;

    auto qmap = fetch_asset_qualities();

    for (auto &row : rows) {
        int id = stoi(row[0]);
        cout << "id: " << id << endl;
        string name = row[1];

        auto q = qmap[id];

        // new_assets.push_back(asset);
        new_assets.emplace_back(id, name, q);
    }

    std::cout << "Number of Assets: " << new_assets.size() << std::endl;

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

    std::cout << "Number of Qualities: " << qualities.size() << std::endl;

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

    std::cout << "Number of Topologies: " << topologies.size() << std::endl;

    return topologies;
}

Keyvalue fetch_facts() {
    Keyvalue initfacts;

    initfacts.populate(fetch_quality_attributes());
    initfacts.populate(fetch_quality_values());
    initfacts.populate(fetch_topology_attributes());
    initfacts.populate(fetch_topology_values());

    std::cout << "Number of Facts: " << initfacts.size() << std::endl;

    return initfacts;
}

void save_ag_to_db(vector<FactbaseItems> &factbase_items,
                   vector<Factbase> &factbases, vector<Edge> &edges,
                   Keyvalue &factlist) {
    string factbase_sql_query = "INSERT INTO factbase VALUES ";

    for (int i = 0; i < factbases.size(); ++i) {
        if (i == 0) {
            factbase_sql_query += "(" + to_string(factbases[i].get_id()) +
                                  ",'" +
                                  to_string(factbases[i].hash(factlist)) + "')";
        } else {
            factbase_sql_query += ",(" + to_string(factbases[i].get_id()) +
                                  ",'" +
                                  to_string(factbases[i].hash(factlist)) + "')";
        }
    }

    factbase_sql_query += " ON CONFLICT DO NOTHING;";

    std::cout << factbase_sql_query << std::endl;
    db.exec(factbase_sql_query);

    string item_sql_query = "INSERT INTO factbase_item VALUES ";
    string quality_sql_query = "";
    string topology_sql_query = "";

    for (int j = 0, sql_index = 0; j < factbase_items.size(); ++j) {
        auto fbi = factbase_items[j];

        int id = get<1>(fbi);
        auto items = get<0>(fbi);

        auto quals = get<0>(items);
        auto topo = get<1>(items);

        for (auto qi : quals) {
            if (sql_index == 0)
                quality_sql_query += "(" + to_string(id) + "," +
                                     to_string(qi.encode(factlist).enc) +
                                     ",'quality')";

            else
                quality_sql_query += ",(" + to_string(id) + "," +
                                     to_string(qi.encode(factlist).enc) +
                                     ",'quality')";
            ++sql_index;
        }

        for (auto ti : topo) {
            if (sql_index == 0)
                topology_sql_query += "(" + to_string(id) + "," +
                                      to_string(ti.encode(factlist).enc) +
                                      ",'topology')";

            else
                topology_sql_query += ",(" + to_string(id) + "," +
                                      to_string(ti.encode(factlist).enc) +
                                      ",'topology')";
            ++sql_index;
        }
    }

    item_sql_query += quality_sql_query + topology_sql_query
                    + " ON CONFLICT DO NOTHING";

    // std::cout << item_sql_query << std::endl;

    // if (topology_sql_query != "")
    //     item_sql_query += quality_sql_query + "," + topology_sql_query +
    //                       " ON CONFLICT DO NOTHING;";
    // else
    //     item_sql_query += quality_sql_query + " ON CONFLICT DO NOTHING;";

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
    for (auto ei : eq) {
        int i = ei.second;

        int eid = edges[i].set_id();

        if (ii == 0) {
            edge_sql_query += "(" + to_string(eid) + "," + ei.first;
            edge_assets_sql_query += edges[i].get_asset_query();
        } else {
            edge_sql_query += ",(" + to_string(eid) + "," + ei.first;
            edge_assets_sql_query += "," + edges[i].get_asset_query();
        }

        ++ii;
    }

    edge_sql_query += " ON CONFLICT DO NOTHING;";
    edge_assets_sql_query += " ON CONFLICT DO NOTHING;";

    db.exec(edge_sql_query);
    db.exec(edge_assets_sql_query);
}

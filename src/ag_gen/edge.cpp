//
// Created by Kyle Cook on 6/24/17.
//

#include <iostream>

#include "edge.h"
#include "util/db.h"

using namespace std;

/**
 * @brief Constructor for Edge
 * @details Initializes information with given parameters and initializes id to 0.
 *
 * @param iFrom The From Node
 * @param iTo The To Node
 * @param ex Exploit associated with the Edge
 * @param ag AssetGroup associated with the Edge
 */
Edge::Edge(int iFrom, int iTo, Exploit &ex, AssetGroup &ag)
    : from_node(iFrom), to_node(iTo), exploit(ex), assetGroup(ag) {
}

/**
 * @brief Returns the Edge ID.
 */
int Edge::get_id() { return id; }

int Edge::get_from_id()
{

    return from_node;

}

int Edge::get_to_id()
{

    return to_node;

}

int Edge::get_exploit_id()
{

    return exploit.get_id();

}

/**
 * @brief Checks if the Edge exists in the database.
 */
// bool Edge::exists_in_db() {
    // vector<Row> rows =
    //     db->exec("SELECT 1 FROM edge WHERE from_node = " +
    //              to_string(from_node) + " AND to_node = " + to_string(to_node) +
    //              " AND exploit_id = " + to_string(exploit.get_id()) + ";");
    // if (!rows.empty()) {
    //     return true;
    // } else {
    //     return false;
    // }
// }

string Edge::get_query()
{

    return to_string(from_node) + "," + to_string(to_node) + "," + to_string(exploit.get_id()) + ")";

}

int Edge::set_id()
{

    id = edge_current_id++;
    return id;

}

int Edge::edge_current_id = 0;

string Edge::get_asset_query()
{

    auto current_perm = assetGroup.get_perm();

    string sql = "";

    for (int i = 0; i < current_perm.size(); ++i)
    {

        if (i == 0)
            sql += "(" + to_string(id) + "," + to_string(i) + "," + to_string(current_perm[i] + 1) + ")";

        else
            sql += ",(" + to_string(id) + "," + to_string(i) + "," + to_string(current_perm[i] + 1) + ")";

    }

    return sql;

}

/**
 * @brief Saves the Edge in the database.
 * @details Saves the Edge in the database and sets the ID of the Edge to the new ID.
 */
// void Edge::save() {
    // if (exists_in_db()) {
    //     vector<Row> rows = db->exec(
    //         "SELECT 1 FROM edge WHERE from_node = " + to_string(from_node) +
    //         " AND to_node = " + to_string(to_node) +
    //         " AND exploit_id = " + to_string(exploit.get_id()) + ";");
    //     id = stoi(rows[0][0]);
    //     return;
    // }

    // vector<Row> rows =
    //     db->exec("SELECT new_edge(" + to_string(from_node) + "," +
    //              to_string(to_node) + "," + to_string(exploit.get_id()) + ");");
    // int factbase_id = stoi(rows[0][0]);
    // id = factbase_id;

    // string sql = "INSERT INTO edge_asset_binding VALUES ";
    // auto current_perm = assetGroup.get_perm();
    // for (auto i = 0; i < current_perm.size(); i++) {
    //     sql += "(" + to_string(id) + "," + to_string(i) + "," +
    //            to_string(current_perm[i] + 1) + ")";
    //     if (i == current_perm.size() - 1) {
    //         sql += ";";
    //     } else {
    //         sql += ",";
    //     }
    // }

    // db->exec(sql);
// }

//
// Created by Kyle Cook on 6/24/17.
//

#include <iostream>

#include "util_db.h"
#include "edge.h"

using namespace std;

Edge::Edge(int iFrom, int iTo, Exploit &ex, AssetGroup &ag) : from_node(iFrom), to_node(iTo), exploit(ex),
                                                              assetGroup(ag) {
    id = 0;
}

int Edge::get_id() {
    return id;
}

// bool Edge::exists_in_db() {
//     ;

//     vector<Row> rows = db->exec(
//             "SELECT 1 FROM edge WHERE from_node = " + to_string(from_node) + " AND to_node = " + to_string(to_node) +
//             " AND exploit_id = " + to_string(exploit.get_id()) + ";");
//     if (rows.size() > 0) {
//         return true;
//     } else {
//         return false;
//     }
// }

// void Edge::save() {
//     ;
//     if (exists_in_db()) {
//         vector<Row> rows = db->exec(
//                 "SELECT 1 FROM edge WHERE from_node = " + to_string(from_node) + " AND to_node = " +
//                 to_string(to_node) + " AND exploit_id = " + to_string(exploit.get_id()) + ";");
//         id = stoi(rows[0][0]);
//         return;
//     }

//     vector<Row> rows = db->exec(
//             "SELECT new_edge(" + to_string(from_node) + "," + to_string(to_node) + "," + to_string(exploit.get_id()) +
//             ");");
//     int factbase_id = stoi(rows[0][0]);
//     id = factbase_id;

//     string sql = "INSERT INTO edge_asset_binding VALUES ";
//     auto current_perm = assetGroup.get_perm();
//     for (auto i = 0; i < current_perm.size(); i++) {
//         sql += "(" + to_string(id) + "," + to_string(i) + "," + to_string(current_perm[i] + 1) + ")";
//         if (i == current_perm.size() - 1) {
//             sql += ";";
//         } else {
//             sql += ",";
//         }
//     }

//     db->exec(sql);
// }

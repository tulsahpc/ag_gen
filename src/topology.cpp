#include <iostream>
#include <string>
#include <vector>

#include "topology.h"
#include "util_db.h"

using namespace std;

Topology::Topology(int f_asset, int t_asset, string opt) : from_asset_id(f_asset), to_asset_id(t_asset), options(opt) {}

int Topology::get_from_asset_id(void) {
    return from_asset_id;
}

int Topology::get_to_asset_id(void) {
    return to_asset_id;
}

string Topology::get_options(void) {
    return options;
}

vector<Topology> Topology::fetch_all() {
    vector<Topology> topologies;

    PGresult *res;
    int num_rows;

    string sql = "SELECT * FROM topology;";

    res = PQexec(conn, sql.c_str());
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "topology SELECT command failed: %s",
                PQerrorMessage(conn));
    }

    num_rows = PQntuples(res);
    for(int i=0; i<num_rows; i++) {
        int from_asset = stoi(PQgetvalue(res, i, 0));
        int to_asset = stoi(PQgetvalue(res, i, 1));
        string options = PQgetvalue(res, i, 2);

        Topology t(from_asset, to_asset, options);
        topologies.push_back(t);
    }

    PQclear(res);
    return topologies;
}

void Topology::print(void) {
    cout << to_string(from_asset_id) + " => " + to_string(to_asset_id) + ": " + options << endl;
}

bool Topology::operator==(const Topology& rhs) {
    if(this->from_asset_id != rhs.from_asset_id)
        return false;
    if(this->to_asset_id != rhs.to_asset_id)
        return false;
    if(this->options != rhs.options)
        return false;
    return true;
}

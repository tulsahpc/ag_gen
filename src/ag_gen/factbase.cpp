// factbase.cpp implements the Factbase class which contains the known facts
// that are completely true in a particular Network State

#include <algorithm>
#include <iostream>
#include <vector>

#include "ag_gen.h"

using namespace std;

Factbase::Factbase()
    : qualities(Quality::fetch_all()), topologies(Topology::fetch_all()) {}

void Factbase::set_parent(const NetworkState &ns) { parent = &ns; }

int Factbase::get_id() const { return id; }

bool Factbase::exists_in_db() {
    string sql =
        "SELECT 1 FROM factbase WHERE hash = '" + to_string(hash()) + "';";
    vector<Row> rows = db->exec(sql);
    if (!rows.empty()) {
        id = stoi(rows[0][0]);
        return true;
    } else {
        return false;
    }
}

// find_quality searches for a given quality in a factbase. Returns true if the
// quality is found, otherwise returns false
bool Factbase::find_quality(Quality &q) const {
    if (find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

// find_topology searches for a given topology in a factbase. Returns true if
// the topology is found, otherwise returns false
bool Factbase::find_topology(Topology &t) const {
    if (find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

// add_quality adds a given quality to the factbase's vector of qualities
void Factbase::add_quality(Quality &q) { qualities.push_back(q); }

// add_topology adds a given topology to the factbase's vector of topologies
void Factbase::add_topology(Topology &t) { topologies.push_back(t); }

void Factbase::save() {
    if (exists_in_db()) {
        return;
    }

    vector<Row> rows =
        db->exec("SELECT new_factbase('" + to_string(hash()) + "');");
    id = stoi(rows[0][0]);

    // XXX: There has to be a better way to do this
    string insert_sql = "INSERT INTO factbase_item VALUES ";
    insert_sql += "(" + to_string(id) + "," +
                  to_string(qualities[0].encode(parent->net->facts).enc) +
                  ",'quality')";
    for (int i = 1; i < qualities.size(); i++) {
        insert_sql += ",(" + to_string(id) + "," +
                      to_string(qualities[i].encode(parent->net->facts).enc) +
                      ",'quality')";
    }
    for (auto &topologie : topologies) {
        insert_sql += ",(" + to_string(id) + "," +
                      to_string(topologie.encode(parent->net->facts).enc) +
                      ",'topology')";
    }
    insert_sql += " ON CONFLICT DO NOTHING;";

    db->exec(insert_sql);
}

// Shamelessly copied from Boost::hash_combine
size_t combine(size_t seed) {
    seed ^= std::hash<size_t>{}(seed) + 0x9e3779b97f4a7c15 + (seed << 6) +
            (seed >> 2);
    return seed;
}

size_t Factbase::hash() const {
    //  size_t hash = 0xf848b64e; // Random seed value
    size_t hash = 0x0c32a12fe19d2119;

    unsigned long qualities_length = qualities.size();
    for (int i = 0; i < qualities_length; i++) {
        auto &qual = qualities.at(i);
        hash = hash ^ combine(qual.encode(parent->net->facts).enc);
    }

    unsigned long topologies_length = topologies.size();
    for (int i = 0; i < topologies_length; i++) {
        auto &topo = topologies.at(i);
        hash = hash ^ combine(topo.encode(parent->net->facts).enc);
    }

    return hash;
}

void Factbase::print() const {
    cout << "ID: " << id << endl;
    cout << "HASH: " << hash() << endl;
    cout << "Qualities: " << qualities.size() << endl;
    cout << "Topologies: " << topologies.size() << endl << endl;
    for (auto &qual : qualities) {
        qual.print();
    }
    for (auto &topo : topologies) {
        topo.print();
    }
}
// factbase.cpp implements the Factbase class which contains the known facts
// that are completely true in a particular Network State

#include <algorithm>
#include <iostream>
#include <vector>

#include "ag_gen.h"

using namespace std;

/**
 * @brief Constructor for Factbase
 * @details Fetches Qualities and Topologies for the Factbase.
 */
Factbase::Factbase()
    : qualities(Quality::fetch_all()), topologies(Topology::fetch_all()) {}

/**
 * @brief Sets the parent NetworkState.
 *
 * @param ns New parent NetworkState
 */
void Factbase::set_parent(const NetworkState &ns) { parent = &ns; }

/**
 * @brief Returns the current Factbase ID.
 */
int Factbase::get_id() const { return id; }

/**
 * @brief Checks if the Factbase exists in the database.
 */
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

/**
 * @brief Searches for a Quality in the Factbase.
 * @details Returns true if the Quality is found and false otherwise.
 *
 * @param q Quality for which to search.
 */
bool Factbase::find_quality(Quality &q) const {
    if (find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

/**
 * @brief Searches for a Topology in the Factbase.
 * @details Returns true if the Topology is found and false otherwise.
 *
 * @param t Topology for which to search.
 */
bool Factbase::find_topology(Topology &t) const {
    if (find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

/**
 * @brief Adds a given Quality to the factbase's vector of Qualities.
 *
 * @param q Quality to add
 */
void Factbase::add_quality(Quality &q) { qualities.push_back(q); }

/**
 * @brief Adds a given Topology to the factbase's vector of Topologies.
 *
 * @param t Topology to add
 */
void Factbase::add_topology(Topology &t) { topologies.push_back(t); }

/**
 * @brief Saves the Factbase to the database.
 */
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

/**
 * @brief Shamelessly copied from Boost::hash_combine
 */
size_t combine(size_t seed) {
    seed ^= std::hash<size_t>{}(seed) + 0x9e3779b97f4a7c15 + (seed << 6) +
            (seed >> 2);
    return seed;
}

/**
 * @brief Hashes the Factbase
 */
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

/**
 * @brief Prints out the Factbase information.
 */
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

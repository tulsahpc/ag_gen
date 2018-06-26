// factbase.cpp implements the Factbase class which contains the known facts
// that are completely true in a particular Network State

#include <algorithm>
#include <iostream>
#include <vector>

#include "ag_gen.h"

using namespace std;

int Factbase::current_id = 0;

/**
 * @brief Constructor for Factbase
 *
 * @param q A vector of Qualities
 * @param t A vector of Topologies
 */
Factbase::Factbase(std::vector<Quality> q, std::vector<Topology> t)
    : qualities(q), topologies(t) {
    id = 0;
}

/**
 * @brief Increments the current ID.
 */
void Factbase::set_id() {
    id = current_id++;
}

/**
 * @return The current Factbase ID.
 */
int Factbase::get_id() const { return id; }

std::tuple<std::vector<Quality>, std::vector<Topology>> Factbase::get_facts_tuple() const {
    return std::make_tuple(qualities, topologies);
}

/**
 * @brief Searches for a Quality in the Factbase.
 * @details Returns true if the Quality is found and false otherwise.
 *
 * @param q Quality for which to search.
 */
bool Factbase::find_quality(Quality &q) const {
    if(std::find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

std::vector<Quality>::iterator Factbase::get_quality(Quality &q) {
    return std::find(qualities.begin(), qualities.end(), q);
}

/**
 * @brief Searches for a Topology in the Factbase.
 * @details Returns true if the Topology is found and false otherwise.
 *
 * @param t Topology for which to search.
 */
bool Factbase::find_topology(Topology &t) const {
    if (std::find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

std::vector<Topology>::iterator Factbase::get_topology(Topology &t) {
    return std::find(topologies.begin(), topologies.end(), t);
}

/**
 * @brief Adds a given Quality to the factbase's vector of Qualities.
 *
 * @param q Quality to add
 */
void Factbase::add_quality(Quality &q) {
    qualities.push_back(q);
}

void Factbase::delete_quality(Quality &q) {
    auto qual = get_quality(q);
    if(qual != qualities.end()) {
        qualities.erase(qual);
    }
}

/**
 * @brief Adds a given Topology to the factbase's vector of Topologies.
 *
 * @param t Topology to add
 */
void Factbase::add_topology(Topology &t) {
    topologies.push_back(t);
}

void Factbase::delete_topology(Topology &t) {
    auto topo = get_topology(t);
    if(topo != topologies.end()) {
        topologies.erase(topo);
    }
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
 *
 * @param factlist The current Keyvalue
 * @return The hash of the Factbase
 */
size_t Factbase::hash(Keyvalue &factlist) const {
    //  size_t hash = 0xf848b64e; // Random seed value
    size_t hash = 0x0c32a12fe19d2119;

    unsigned long qualities_length = qualities.size();
    for (int i = 0; i < qualities_length; i++) {
        auto &qual = qualities.at(i);
        hash = hash + combine(qual.encode(factlist).enc);
    }

    unsigned long topologies_length = topologies.size();
    for (int i = 0; i < topologies_length; i++) {
        auto &topo = topologies.at(i);
        hash = hash + combine(topo.encode(factlist).enc);
    }

//    std::cout << "Hash: " << hash << std::endl;
    return hash;
}

/**
 * @brief Prints out the Factbase information.
 */
void Factbase::print() const {
    cout << "ID: " << id << endl;
//        cout << "HASH: " << hash() << endl;
    cout << "Qualities: " << qualities.size() << endl;
    cout << "Topologies: " << topologies.size() << endl << endl;
    for (auto &qual : qualities) {
        qual.print();
    }
    for (auto &topo : topologies) {
        topo.print();
    }
}

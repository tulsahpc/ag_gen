//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

/**
 * @brief Normal Constructor for NetworkState
 * @details Creates a Factbase for a given Network
 *          and sets this as its parent.
 *
 * @param net The Network for which to create a state
 */
NetworkState::NetworkState(std::vector<Quality> q, std::vector<Topology> t)
    : factbase(Factbase(q, t)) {
    factbase.set_parent(*this);
}

/**
 * @brief Copy Constructor for NetworkState
 * @details Creates a new Factbase and sets Network
 *          based on an incoming NetworkState
 *
 * @param ns The NetworkState from which to copy
 */
NetworkState::NetworkState(const NetworkState &ns)
    : factbase(ns.factbase) {
    factbase.set_parent(*this);
}

void NetworkState::set_id()
{

    factbase.set_id();

}

/**
 * @brief Returns the Factbase for the NetworkState
 */
const Factbase &NetworkState::get_factbase() const { return factbase; }

/**
 * @brief Returns the hash of the Factbase
 */
size_t NetworkState::get_hash(Keyvalue &factlist) const { return factbase.hash(factlist); }

/**
 * @brief Adds all unique Quality elements of a vector to the Factbase
 *
 * @param q A vector of Qualities
 */
void NetworkState::add_qualities(std::vector<Quality> q) {
    // For each quality, check if it already exists in the factbase. If it does
    // not already exist, we add it.
    for (auto &qual : q) {
        if (!factbase.find_quality(qual)) {
            factbase.add_quality(qual);
        }
    }
}

/**
 * @brief Adds all unique Topology elements of a vector to the Factbase
 *
 * @param t A vector of Topologies
 */
void NetworkState::add_topologies(std::vector<Topology> t) {
    // For each topology, check if it already exists in the factbase. If it does
    // not already exist, we add it.
    for (auto &topo : t) {
        if (!factbase.find_topology(topo)) {
            factbase.add_topology(topo);
        }
    }
}

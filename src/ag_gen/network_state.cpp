//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

/**
 * @brief Normal Constructor for NetworkState
 * @details Creates a Factbase for a given Network
 *          and sets this as its parent.
 *
 * @param q A vector of Qualities
 * @param t A vector of Topologies
 */
NetworkState::NetworkState(std::vector<Quality> q, std::vector<Topology> t)
    : factbase(Factbase(q, t)) {}

/**
 * @brief Copy Constructor for NetworkState
 * @details Creates a new Factbase and sets Network
 *          based on an incoming NetworkState
 *
 * @param ns The NetworkState from which to copy
 */
NetworkState::NetworkState(const NetworkState &ns) : factbase(ns.factbase){}

/**
 * @brief Sets the ID of the Factbase
 */
void NetworkState::set_id() { factbase.set_id(); }

/**
 * @return The ID of the NetworkState
 */
int NetworkState::get_id() { return factbase.get_id(); }

/**
 * @return The Factbase for the NetworkState
 */
const Factbase &NetworkState::get_factbase() const { return factbase; }

/**
 * @brief Returns the hash of the Factbase
 *
 * @param factlist The current Keyvalue
 * @return The hash of the Factbase
 */
size_t NetworkState::get_hash(Keyvalue &factlist) const {
    return factbase.hash(factlist);
}

/**
 * @brief Adds all unique Quality elements of a vector to the Factbase
 *
 * @param q A vector of Qualities
 */
void NetworkState::add_qualities(std::vector<Quality> q) {
    // For each quality, check if it already exists in the factbase. If it does
    // not already exist, we add it.
    for (auto &qual : q) {
        add_quality(qual);
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
        add_topology(topo);
    }
}

void NetworkState::add_quality(Quality &q) {
    if (!factbase.find_quality(q)) {
        factbase.add_quality(q);
    }
}

void NetworkState::add_topology(Topology &t) {
    if (!factbase.find_topology(t)) {
        factbase.add_topology(t);
    }
}

void NetworkState::update_quality(Quality &q) {
    if(factbase.find_quality(q)) {
        auto asset_id = q.get_asset_id();
        auto attr = q.get_name();
        auto val = q.get_value();

        for(auto &qual : factbase.qualities) {
            if(qual.get_asset_id() == asset_id &&
                qual.get_name() == attr) {
                qual.set_value(val);
            }
        }
    }
}

void NetworkState::update_topology(Topology &t) {
    if(factbase.find_topology(t)) {
        delete_topology(t);
        add_topology(t);
    }
}

void NetworkState::delete_quality(Quality &q) {
    auto qual = factbase.get_quality(q);
    if(qual != factbase.qualities.end()) {
        factbase.qualities.erase(qual);
    }
}

void NetworkState::delete_topology(Topology &t) {
    auto topo = factbase.get_topology(t);
    if(topo != factbase.topologies.end()) {
        factbase.topologies.erase(topo);
    }
}

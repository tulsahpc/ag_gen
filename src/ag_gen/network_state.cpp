//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

NetworkState::NetworkState(Network &net) {
    factbase.set_parent(*this);
    this->net = &net;
}

NetworkState::NetworkState(const NetworkState &ns)
    : net(ns.net), factbase(ns.factbase) {
    factbase.set_parent(*this);
}

const Factbase &NetworkState::get_factbase() const { return factbase; }

size_t NetworkState::get_hash() const { return factbase.hash(); }

void NetworkState::add_qualities(std::vector<Quality> q) {
    // For each quality, check if it already exists in the factbase. If it does
    // not already exist, we add it.
    for (auto &qual : q) {
        if (!factbase.find_quality(qual)) {
            factbase.add_quality(qual);
        }
    }
}

void NetworkState::add_topologies(std::vector<Topology> t) {
    // For each topology, check if it already exists in the factbase. If it does
    // not already exist, we add it.
    for (auto &topo : t) {
        if (!factbase.find_topology(topo)) {
            factbase.add_topology(topo);
        }
    }
}
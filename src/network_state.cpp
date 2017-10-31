//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

NetworkState::NetworkState() : all_attrs(Quality::fetch_all_attributes()), all_vals(Quality)

Factbase NetworkState::get_factbase() const {
    return factbase;
}

size_t NetworkState::get_num_assets() const {
    return assets.size();
}

size_t NetworkState::get_hash() const {
    return factbase.hash();
}

void NetworkState::add_qualities(std::vector<Quality> q) {
    // For each quality, check if it already exists in the factbase. If it does not already exist, we add it.
    for (auto &qual : q) {
        if (!factbase.find_quality(qual)) {
            factbase.add_quality(qual);
        }
    }
}

void NetworkState::add_topologies(std::vector<Topology> t) {
    // For each topology, check if it already exists in the factbase. If it does not already exist, we add it.
    for (auto &topo : t) {
        if (!factbase.find_topology(topo)) {
            factbase.add_topology(topo);
        }
    }
}

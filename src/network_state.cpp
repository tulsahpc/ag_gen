//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"
#include "asset.h"

NetworkState::NetworkState() :
        all_assets(Asset::fetch_all("home")),
		all_attrs(move(populate_attrs())),
		all_vals(move(populate_vals())) {
    factbase.set_parent(*this);
    kv_facts.populate(all_attrs);
    kv_facts.populate(all_vals);
}

const Factbase &NetworkState::get_factbase() const {
    return factbase;
}

size_t NetworkState::get_num_assets() const {
    return all_assets.size();
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

std::vector<std::string> NetworkState::populate_attrs() {
	std::vector<std::string> fact_attrs {Quality::fetch_all_attributes()};
	std::vector<std::string> attr_temp {Topology::fetch_all_attributes()};
	fact_attrs.insert(
		fact_attrs.end(),
		std::make_move_iterator(attr_temp.begin()),
		std::make_move_iterator(attr_temp.end())
	);
	return fact_attrs;
}

std::vector<std::string> NetworkState::populate_vals() {
	std::vector<std::string> fact_vals {Quality::fetch_all_values()};
	std::vector<std::string> val_temp {Topology::fetch_all_values()};
	fact_vals.insert(
		fact_vals.end(),
		std::make_move_iterator(val_temp.begin()),
		std::make_move_iterator(val_temp.end())
	);
	return fact_vals;
}

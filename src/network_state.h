// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include <vector>
#include <string>

#include "quality.h"
#include "factbase.h"
#include "util_db.h"

class NetworkState {
    Factbase factbase;
	std::string network = "home";

	std::vector<std::string> all_attrs;
	std::vector<std::string> all_vals;

	Keyvalue<std::string> attrs_kv;
	Keyvalue<std::string> vals_kv;

	friend class Quality;
	friend class Topology;
public:
	NetworkState();
	explicit NetworkState(Factbase &fb);
    Factbase get_factbase() const;

	void init_qualities();
	void init_topologies();


};

#endif

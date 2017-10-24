// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include <vector>
#include <string>

#include "factbase.h"
#include "keyvalue.h"

class NetworkState {
    Factbase factbase;
	std::string network;

	std::vector<std::string> all_attrs;
	std::vector<std::string> all_vals;

	Keyvalue<std::string> attrs_kv;
	Keyvalue<std::string> vals_kv;

public:
    Factbase get_factbase() const;

	void init_qualities();
//	void init_topologies();
};

#endif

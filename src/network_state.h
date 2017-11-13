// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "asset.h"
#include "quality.h"
#include "topology.h"
#include "factbase.h"
#include "keyvalue.h"

class NetworkState {
    Factbase factbase;
    Keyvalue kv_facts;

    friend class Factbase;

public:
    NetworkState();
    NetworkState(const NetworkState &ns);

    const Factbase &get_factbase() const;
    size_t get_hash() const;

    void add_qualities(std::vector<Quality> q);
    void add_topologies(std::vector<Topology> t);
};

#endif

// network_state.h implements the Network State class which represents a single
// state of a network and cotains a factbase of known truths for the Network
// State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "asset.h"
#include "factbase.h"
#include "quality.h"
#include "topology.h"

#include "util/keyvalue.h"

class Network;

/** NetworkState class
 * @brief Manages the current state of the network
 * @details The current network state is dependent on the Qualities
 *          and Topologies in the Factbase. NetworkState allows
 *          for the addition of Qualities and Topologies to the Factbase.
 */
class NetworkState {
    Network *net;
    Factbase factbase;
    friend class Factbase;

  public:
    NetworkState(Network &net, std::vector<Quality> q, std::vector<Topology> t);
    NetworkState(const NetworkState &ns);

    const Factbase &get_factbase() const;
    size_t get_hash() const;

    void add_qualities(std::vector<Quality> q);
    void add_topologies(std::vector<Topology> t);
};

#endif

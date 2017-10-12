// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    const Factbase factbase;
public:
    explicit NetworkState(const Factbase &fb);

    explicit NetworkState(Factbase &&fb);

    Factbase get_factbase();
};

#endif

// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
    // default constructor that creates a NetworkState object with a zero-value Factbase
    NetworkState(void) {};

    // copy constructor that creates a new NetworkState with the same factbase as the given NetworkState
    NetworkState(const NetworkState& ns): factbase(const_cast<NetworkState&>(ns).get_factbase()) {};

    // get_factbase returns the factbase of a NetworkState
    const Factbase& get_factbase(void) const {
        return factbase;
    };

    // print prints the factbase of the NetworkState by calling on factbase's print method
    void print(void) const {
        factbase.print();
    };
};

#endif

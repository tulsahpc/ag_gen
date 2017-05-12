// network_state.h contains standard constructors, get method for the factbase, and a print method that prints
// the factbase
#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
    // default constructor
    NetworkState(void) {};

    // copy constructor that creates a new NetworkState with the same factbse as the given NetworkState
    NetworkState(const NetworkState& ns): factbase(const_cast<NetworkState&>(ns).get_factbase()) {};

    // get_factbase returns the factbase of a NetworkState
    Factbase& get_factbase(void) {
        return factbase;
    };

    // print prints the factbase of the NetworkState by calling on factbase's print method
    void print(void) {
        factbase.print();
    };
};

#endif

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
    NetworkState(void) {};

    NetworkState(NetworkState& ns) : factbase(ns.get_factbase()) {};

    Factbase& get_factbase(void) {
        return factbase;
    };

    void print(void) {
        factbase.print();
    };
};

#endif

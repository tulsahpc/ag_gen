#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
    NetworkState(void) {};
    NetworkState(const NetworkState& ns): factbase(const_cast<NetworkState&>(ns).get_factbase()) {};

    const Factbase& get_factbase(void) const {
        return factbase;
    };

    void print(void) const {
        factbase.print();
    };
};

#endif

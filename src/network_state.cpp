//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

// default constructor that creates a NetworkState object with a zero-value Factbase
NetworkState::NetworkState(Factbase &fb) : factbase(fb) {}

Factbase NetworkState::get_factbase() const {
    return factbase;
}

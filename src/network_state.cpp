//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

// default constructor that creates a NetworkState object with a zero-value Factbase
//NetworkState::NetworkState(void) : factbase() {};

NetworkState::NetworkState(const Factbase &fb) : factbase(fb) {};

NetworkState::NetworkState(Factbase &&fb) : factbase(std::move(fb)) {}

Factbase NetworkState::get_factbase() {
    return factbase;
}

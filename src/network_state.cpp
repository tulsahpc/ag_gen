//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

// default constructor that creates a NetworkState object with a zero-value Factbase
NetworkState::NetworkState(void) : factbase() {};

NetworkState::NetworkState(Factbase& fb) : factbase(fb) {};

// copy constructor that creates a new NetworkState with the same factbase as the given NetworkState
NetworkState::NetworkState(const NetworkState& ns) : factbase(const_cast<NetworkState&>(ns).get_factbase()) {};

Factbase& NetworkState::get_factbase(void) {
	return factbase;
}

void NetworkState::init(void) {
	factbase.populate();
}
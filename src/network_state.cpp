//
// Created by Kyle Cook on 6/26/17.
//

#include "network_state.h"

// default constructor that creates a NetworkState object with a zero-value Factbase
NetworkState::NetworkState(void) : factbase() {
	factbase.populate();
	factbase.save();
};

NetworkState::NetworkState(Factbase& fb) : factbase(fb) {}

// copy constructor that creates a new NetworkState with the same factbase as the given NetworkState
NetworkState::NetworkState(const NetworkState& ns) {}

Factbase& NetworkState::get_factbase(void) {
	return factbase;
}

// print prints the factbase of the NetworkState by calling on factbase's print method
void NetworkState::print(void) const {
	factbase.print();
};

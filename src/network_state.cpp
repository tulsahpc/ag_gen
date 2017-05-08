#include "network_state.h"

#include "factbase.h"

NetworkState::NetworkState(void) {}

Factbase& NetworkState::get_factbase(void) {
    return this->factbase;
}
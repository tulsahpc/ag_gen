// network_state.h implements the Network State class which represents a single state of a network and cotains a factbase of known truths for the Network State

#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
	NetworkState(void);
	NetworkState(Factbase& fb);
	NetworkState(const NetworkState& ns);

	Factbase& get_factbase(void);
	const Factbase& get_factbase(void) const;

	void init(void);
};

#endif

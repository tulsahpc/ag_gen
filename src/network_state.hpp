#ifndef NETWORK_STATE_HPP
#define NETWORK_STATE_HPP

class NetworkState {
	int id;
	int network_id;
	int parent_id;
public:
	NetworkState(Network &);
};

#endif // NETWORK_STATE_HPP

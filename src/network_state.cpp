#include "network.hpp"
#include "network_state.hpp"

NetworkState::NetworkState(Network &net)
{
	id = net.id;
	network_id = net.id;
	parent_id = 0;
}

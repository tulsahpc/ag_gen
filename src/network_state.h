#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "asset.h"
#include "util_list.h"

struct NetworkState {
	int network_id;
	struct List assets;
};

#endif

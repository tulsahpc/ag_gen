#include "ag_asset.h"
#include "util_list.h"

struct AGNetworkState {
	int network_id;
	struct List assets;
};

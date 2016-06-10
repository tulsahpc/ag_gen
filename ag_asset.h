#ifndef C_AG_ASSET_H
#define C_AG_ASSET_H

#include "ag_facts.h"

struct AGAssetList {
	struct AGAsset **assets;
	int len;
};

struct AGAsset {
	int id;
	char *name;
	int network_id;
	//struct AGNetwork* network;
};

struct AGAssetList *AGGetAssets(void);
struct AGAsset *AGAssetNew(int, char *);
int AGAssetFree(struct AGAsset *);
void AGAssetsPrint(const struct AGAssetList *);

#endif //C_AG_ASSET_H

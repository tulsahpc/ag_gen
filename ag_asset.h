//
// Created by Kyle Cook on 5/26/16.
//

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
int AGAssetsFree(struct AGAssetList *);
void AGAssetsPrint(const struct AGAssetList *);

union AGFactList *AGAssetGetFacts(const struct AGAsset *);

#endif //C_AG_ASSET_H

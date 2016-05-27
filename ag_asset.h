//
// Created by Kyle Cook on 5/26/16.
//

#ifndef C_AG_ASSET_H
#define C_AG_ASSET_H

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
int AGFreeAssets(struct AGAssetList *assetList);
void printAssets(const struct AGAssetList*);

#endif //C_AG_ASSET_H

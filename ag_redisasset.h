#ifndef C_AG_REDISASSET_H
#define C_AG_REDISASSET_H

#include "ag_asset.h"
#include "r_client.h"

struct RedisAsset {
    struct AGAsset *asset;
    struct RCList *rclist;
};

void AddRedisAsset(struct AGAsset *, struct RCList *);
//void AddRedisAssets(struct AGAssetList *, struct RCList *);
struct AGAsset * GetRedisAsset(struct RCList *);
//struct AGAssetList * GetRedisAssetList(struct RCList *);

#endif //C_AG_REDISASSET_H

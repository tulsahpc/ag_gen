#ifndef C_AG_REDISASSET_H
#define C_AG_REDISASSET_H



void AddRedisAsset(struct AGAsset *asset, struct RList *rlist);
void AddRedisAssets(struct AGAssetList *assetlist, struct RList *rlist);
struct AGAsset * GetRedisAsset(struct Rlist *rlist);
struct AGAssetList * GetRedisAssetList(struct Rlist *rlist);

#endif //C_AG_REDISASSET_H

#ifndef C_AG_REDISASSET_H
#define C_AG_REDISASSET_H



void AddRedisAsset(struct AGAsset *, struct RList *);
void AddRedisAssets(struct AGAssetList *, struct RList *);
struct AGAsset * GetRedisAsset(struct RList *);
struct AGAssetList * GetRedisAssetList(struct RList *);

#endif //C_AG_REDISASSET_H

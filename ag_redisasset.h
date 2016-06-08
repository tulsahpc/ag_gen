#ifndef C_AG_REDISASSET_H
#define C_AG_REDISASSET_H

#include "ag_asset.h"
#include "redis_util.h"

int RedisAssetAdd(const char *key, struct AGAsset *);
struct AGAsset *RedisAssetGet(const char *key);

#endif //C_AG_REDISASSET_H

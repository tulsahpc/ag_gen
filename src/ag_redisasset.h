/** \file ag_redisasset.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef C_AG_REDISASSET_H
#define C_AG_REDISASSET_H

#include "ag_asset.h"
#include "redis_util.h"

int RedisAssetAdd(const char *key, struct AGAsset *);
struct AGAsset *RedisAssetGet(const char *key);

#endif //C_AG_REDISASSET_H

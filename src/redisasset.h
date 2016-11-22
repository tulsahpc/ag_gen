/** \file ag_redisasset.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef REDISASSET_H
#define REDISASSET_H

#include "asset.hpp"
//#include "util_redis.h"

int rasset_set(const char *key, Asset *);
Asset *rasset_get(const char *key);

#endif //REDISASSET_H

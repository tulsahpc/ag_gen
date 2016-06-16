/** \file ag_redisasset.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 *
 * Handles the asset to redis server interchange.
 *
 * Takes an assets and turns it into a string that can be put into the redis server.
 * Retrives data from a redis server list and turns it into an asset that can be returned.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag_redisasset.h"
#include "ag_asset.h"
#include "redis_util.h"
#include "util.h"

/*!
 * Maxlength of the strings that can be inputed/outputed
 */
#define MAXSTRLEN 128
#define DELIMITER ":"

/**
 * Takes in an asset and key and adds the assets data into the redis list given by key.
 *
 * Will print to the console and return 1 if the asset is null.
 */
int RedisAssetAdd(const char *key, struct AGAsset *asset)
{
	char *str;

	if(key == NULL)
		return 1;

	if(asset == NULL)
		return 1;

	str = malloc(sizeof(char)*(MAXSTRLEN+1));
	snprintf(str, MAXSTRLEN, "%d:%s:%d:%s:%s", asset->id, asset->name, asset->network_id, asset->qualities, asset->topologies);

	RedisEnqueueValue(key, str);
	free(str);

	return 0;
}

/**
 * Gets the asset out of a redis list given by key.
 *
 * If the list given by key is empty then then it will return a null asset
 */
struct AGAsset *RedisAssetGet(const char *key)
{
	char *str;
	char *saveptr;
	struct AGAsset *asset = {0};

	if(key == NULL)
		return NULL;

	str = RedisDequeueValue(key);
	DEBUG_PRINT("key: %s\n", key);
	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", str);

		asset = malloc(sizeof(struct AGAsset));
		asset->id = atoi(strtok_r(str, DELIMITER, &saveptr));

		char *assetName = strtok_r(NULL, DELIMITER, &saveptr);
		asset->name = dynstr(assetName);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));

		char *qualitiesKey = strtok_r(NULL, DELIMITER, &saveptr);
		char *topologiesKey = strtok_r(NULL, DELIMITER, &saveptr);
		asset->qualities = dynstr(qualitiesKey);
		asset->topologies = dynstr(topologiesKey);

		free(str);
	}

	return asset;
}

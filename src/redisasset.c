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

#include "asset.h"
#include "redisasset.h"
#include "util_redis.h"
#include "util_common.h"

/*!
 * Maxlength of the strings that can be inputed/outputed
 */
#define DELIMITER ":"

/**
 * Takes in an asset and key and adds the assets data into the redis list given by key.
 *
 * Will print to the console and return 1 if the asset is null.
 */
int rasset_set(const char *key, struct Asset *asset)
{
	char *str;

	if(key == NULL)
		return 1;

	if(asset == NULL)
		return 1;

	str = malloc(sizeof(char)*(MAXSTRLEN+1));
	snprintf(str, MAXSTRLEN, "%d:%s:%d", asset->id, asset->name, asset->network_id);

	redis_enqueue(key, str);
	free(str);

	return 0;
}

/**
 * Gets the asset out of a redis list given by key.
 *
 * If the list given by key is empty then then it will return a null asset
 */
struct Asset *rasset_get(const char *key)
{
	char *str;
	char *saveptr;
	struct Asset *asset = {0};

	if(key == NULL)
		return NULL;

	str = redis_dequeue(key);
	DEBUG_PRINT("key: %s\n", key);
	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", str);

		asset = malloc(sizeof(struct Asset));
		asset->id = atoi(strtok_r(str, DELIMITER, &saveptr));

		char *assetName = strtok_r(NULL, DELIMITER, &saveptr);
		asset->name = dynstr(assetName);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));

		free(str);
	}

	return asset;
}

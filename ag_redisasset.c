#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag_redisasset.h"
#include "ag_asset.h"
#include "redis_util.h"
#include "util.h"

int RedisAssetAdd(const char *key, struct AGAsset *asset)
{
	char *str;

	if(key == NULL)
		return 1;

	if(asset == NULL)
		return 1;

	str = malloc(sizeof(char)*MAXSTRLEN);
	snprintf(str, MAXSTRLEN, "%d:%s:%d", asset->id, asset->name, asset->network_id);

	RedisEnqueueValue(key, str);
	free(str);

	return 0;
}

struct AGAsset *RedisAssetGet(const char *key)
{
	char *str;
	char *saveptr;
	struct AGAsset *asset = {0};

	str = RedisDequeueValue(key);

	DEBUG_PRINT("key: %s\n", key);

	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", str);

		asset = malloc(sizeof(struct AGAsset));
		asset->id = atoi(strtok_r(str, DELIMITER, &saveptr));

		char *assetName = strtok_r(NULL, DELIMITER, &saveptr);
		asset->name = dynstr(assetName);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));

		free(str);
	}

	return asset;
}

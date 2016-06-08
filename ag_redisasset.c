#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag_redisasset.h"
#include "ag_asset.h"
#include "redis_util.h"
#include "util.h"

#define MAXSTRLEN 128
#define DELIMITER ":"

int RedisAssetAdd(const char *key, struct AGAsset *asset)
{
	char *str;
	int written;

	if(asset == NULL) {
		printf("Asset cannot be null.");
		return 1;
	}

	str = malloc(sizeof(char)*(MAXSTRLEN+1));
	written = snprintf(str, MAXSTRLEN, "%d:%s:%d", asset->id, asset->name, asset->network_id);

	if(written < MAXSTRLEN) {
		str[written] = '\0';
	} else {
		str[MAXSTRLEN] = '\0';
	}

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
		asset->name = strtok_r(NULL, DELIMITER, &saveptr);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));

		free(str);
	}

	return asset;
}

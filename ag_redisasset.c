#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag_redisasset.h"
#include "ag_asset.h"
#include "r_client.h"


#define DELIMITER ":"

void AddRedisAsset(struct AGAsset *asset, struct RList *rlist)
{
	char* str;

	snprintf(str, "%d:", asset->id);
	snprintf(str[strlen(str)+ 1], "%s:", asset->name);
	snprintf(str[strlen(str) + 1], "%d", asset->network_id);

	RedisEnqueueValue(rlist->c, rlist->key, ParseAssetToString(asset, str));
}

void AddRedisAssets(struct AGAssetList *assets, struct RList *rlist)
{
	int i;
	for (i = 0; i < assets->len; i++){
		AddRedisAsset(assets[i], rlist);
	}

}

struct AGAsset * GetRedisAsset(struct RList *rlist)
{
	char *str;
	char *saveptr;
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));

	str = RedisDequeueValue(rlist->c,rlist->key);
	str[strlen(str)] = '\0';

	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", str);

		asset->id = atoi(strtok_r(str, DELIMITER, &saveptr));
		asset->name = strtok_r(NULL, DELIMITER, &saveptr);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));
	}

	return asset;
}

struct AGAssetList * GetRedisAssetList(struct RList *rlist)
{
	int i, listsize;
	listsize = RedisListLength(rlist->c,rlist->key);
	struct AGAssetList *assetList;
	struct AGAsset **assets = malloc(sizeof(struct AGAsset)*listsize);

	for (i = 0; i < listsize; i++){
		assets[i] = AGetRedisAsset(rlist);
	}

	assetList = malloc(sizeof(struct AGAssetList));
	assetList->assets = assets;
	assetList->len = RedisListLength(rlist->c, rlist->key);

	return assetList;
}


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
	snprintf(str[++strlen(str)], "%s:", asset->name);
	snprintf(str[++strlen(str)], "%d", asset->network_id);

	RedisEnqueueValue(rlist->c, rlist->key, ParseAssetToString(asset->asset, str));
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
	char* str;
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));

	str = RedisDequeueValue(c,key);
	str[reply->len] = '\0';

	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", assetstr);

		asset->id = atoi(strtok_r(assetstr, DELIMITER, &saveptr));
		asset->name = strtok_r(NULL, DELIMITER, &saveptr);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));
	}

	return asset;
}

struct AGAssetList * GetRedisAssetList(struct Rlist* rlist)
{
	int i;
	struct AGAssetList *assetList;
	struct AGAsset **assets = malloc(sizeof(AGAsset)*RedisListLength(rlist->c,rlist->key));

	for (i = 0; i < assets->len; i++){
		assets[i] = AGetRedisAsset(rlist);
	}

	assetList = malloc(sizeof(struct AGAssetList));
	assetList->assets = assets;
	assetList->len = numRows;

	return assetList;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ag_redisasset.h"
#include "ag_asset.h"
#include "r_client.h"
#include "util.h"


#define MAXSTRLEN 128
#define DELIMITER ":"

void AddRedisAsset(struct AGAsset *asset, struct RCList *rclist)
{
	char *str = malloc(sizeof(char)*MAXSTRLEN);
	int i = 0;
	snprintf(str, MAXSTRLEN, "%d:", asset->id);
	i = strlen(str) + 1;
	snprintf(&str[i], MAXSTRLEN, "%s:", asset->name);
	i = strlen(str) + 1;
	snprintf(&str[i], MAXSTRLEN, "%d", asset->network_id);

	RedisEnqueueValue(rclist->c, rclist->key, str);
}
/*
void AddRedisAssets(struct AGAssetList *assets, struct RCList *rclist)
{
	int i;
	for (i = 0; i < assets->len; i++){
		AddRedisAsset(assets[i], rclist);
	}

}*/

struct AGAsset * GetRedisAsset(struct RCList *rclist)
{
	char *str = malloc(sizeof(char)*MAXSTRLEN);
	char *saveptr;
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));

	str = RedisDequeueValue(rclist->c,rclist->key);
	str[strlen(str)] = '\0';

	if(str != NULL){
		DEBUG_PRINT("DEQUEUE Reply string is: %s\n", str);

		asset->id = atoi(strtok_r(str, DELIMITER, &saveptr));
		asset->name = strtok_r(NULL, DELIMITER, &saveptr);
		asset->network_id = atoi(strtok_r(NULL, DELIMITER, &saveptr));
	}

	return asset;
}
/*
struct AGAssetList * GetRedisAssetList(struct RCList *rclist)
{
	int i, listsize;
	listsize = RedisListLength(rclist->c,rclist->key);
	struct AGAssetList *assetList;
	struct AGAsset **assets = malloc(sizeof(struct AGAsset)*listsize);

	for (i = 0; i < listsize; i++){
		assets[i] = AGetRedisAsset(rclist);
	}

	assetList = malloc(sizeof(struct AGAssetList));
	assetList->assets = assets;
	assetList->len = RedisListLength(rclist->c, rclist->key);

	return assetList;
}
*/

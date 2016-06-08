#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "r_client.h"
#include "ag_redisasset.h"
#include "ag_asset.h"

int main()
{
	struct RCList *rclist = malloc(sizeof(struct RCList));
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));
	struct AGAsset *newasset = malloc(sizeof(struct AGAsset));

	RedisConnect(rclist->c);
	rclist->key = "assetList";

	asset->id = 1234;
	asset->name = "Sheard Dumisani";
	asset->network_id = 19122232;

	printf("Asset set\n");
	printf("Asset details : ID %d : NAME %s : NETWORK_ID %d\n", asset->id, asset->name, asset->network_id);


	AddRedisAsset(asset,rclist);
	printf("Asset enqueued\n");

	newasset = GetRedisAsset(rclist);
	printf("Asset dequeued\n");
	printf("Asset details after dequeue : ID %d : NAME %s : NETWORK_ID %d\n", newasset->id, newasset->name, newasset->network_id);

	AGAssetFree(asset);
	AGAssetFree(newasset);
	return 0;
}

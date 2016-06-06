#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "ag_redisconnect.h"
#include "ag_redisclient.h"
#include "ag_asset.h"

int main()
{
	redisContext *c;
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));
	struct AGAsset *newasset = malloc(sizeof(struct AGAsset));

	c = RedisConnect(c);
	RedisSetContext(c);
	RedisSetListName("testlist");

	asset->id = 1234;
	asset->name = "Sheard Dumisani";
	asset->network_id = 19122232;

	printf("Asset set\n");
	printf("Asset details : ID %d : NAME %s : NETWORK_ID %d\n", asset->id, asset->name, asset->network_id);

	RedisPing();

	RedisEnqueueAsset(asset);
	printf("Asset enqueued\n");

	newasset = RedisDequeueAsset();
	printf("Asset dequeued\n");
	printf("Asset details after dequeue : ID %d : NAME %s : NETWORK_ID %d\n", newasset->id, newasset->name, newasset->network_id);

	RedisFree(c);
	AGAssetsFree(asset);
	AGAssetsFree(newasset);
	return 0;
}

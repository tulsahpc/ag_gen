#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "redis_util.h"
#include "ag_redisasset.h"
#include "ag_asset.h"
#include "util.h"

redisContext *cxt;

int main()
{
	struct AGAsset *asset = malloc(sizeof(struct AGAsset));

	asset->id = 1234;
	asset->name = "Sheard Dumisani";
	asset->network_id = 19122232;

	int con = RedisConnect();
	if(con != 0) {
		exit(1);
	}

	RedisAssetAdd("asset1", asset);
	DEBUG_PRINT("Asset Enqueued\n",0);

	struct AGAsset *newAsset = RedisAssetGet("asset1");
	DEBUG_PRINT("Asset Dequeued\n",0);

	AGAssetFree(asset);
	AGAssetFree(newAsset);
	return 0;
}

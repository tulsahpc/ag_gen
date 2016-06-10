/**  \file
 * Redis_test determines if there is a usable redis server.
 *
 * When run this executable validates whether a connection can be established to the redis server
 * and if an asset that is put into the redis server can be retrieved with all of its data intact.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "redis_util.h"
#include "ag_redisasset.h"
#include "ag_asset.h"
#include "util.h"

/**
 * Global redis server context. For testing purposes only.
 */
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

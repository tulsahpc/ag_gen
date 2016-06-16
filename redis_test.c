/** \file redis_test.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 *
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
#include "ag_redisexploit.h"
#include "ag_exploit.h"
#include "util.h"

int main()
{
	int res;
	struct AGAsset *asset = calloc(1, sizeof(struct AGAsset));

	asset->id = 1234;
	asset->name = dynstr("Sheard Dumisani");;
	asset->network_id = 19122232;

	int con = RedisConnect();
	if(con != 0) {
		exit(1);
	}

	RedisAssetAdd("asset1", asset);
	DEBUG_PRINT("Asset Enqueued\n",0);

	struct AGAsset *newAsset = RedisAssetGet("asset1");
	DEBUG_PRINT("Asset Dequeued\n",0);

	res = AGAssetFree(asset);
	if(res != 0) {
		printf("There was a problem.");
		exit(1);
	}

	res = AGAssetFree(newAsset);
	if(res != 0) {
		printf("There was a problem.");
		exit(1);
	}

	// struct AGExploit *exploit = malloc(sizeof(struct AGExploit));
	// exploit->id = 143222;
	// exploit->name = "exploit1";
	// ExploitPrint(exploit);

	// RedisExploitAdd("exploit", exploit);
	// DEBUG_PRINT("Exploit Enqueued\n",0);

	// struct AGExploit *ex2 = RedisExploitGet("exploit");
	// DEBUG_PRINT("Exploit Dequeued\n",0);

	// ExploitPrint(ex2);

	// free(exploit);
	// free(ex2);

	return 0;
}

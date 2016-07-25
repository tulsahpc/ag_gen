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

int RedisAsset_Test(void);
int RedisExploit_Test(void);

int main()
{
	int res;
	int state = 0;
	int error = 0;

	res = RedisConnect();
	if(res != 0) {
		DEBUG_PRINT("Error connecting to redis.\n",0);
		exit(1);
	}

	res = RedisAsset_Test();
	if(res) {
		state = 1;
		error = 1;
	}
	TEST_PRINT("RedisAsset_Test", state);
	state = 0;

	res = RedisExploit_Test();
	if(res) {
		state = 1;
		error = 1;
	}
	TEST_PRINT("RedisExploit_Test", state);
	state = 0;

	return error;
}

int RedisAsset_Test()
{
	int res;
	struct AGAsset *asset;
	struct AGAsset *new_asset;

	asset = calloc(1, sizeof(struct AGAsset));
	if(asset == NULL) {
		DEBUG_PRINT("Memory allocation error.\n",0);
		exit(1);
	}

	asset->id = 1234;
	asset->name = dynstr("Sheard Dumisani");;
	asset->network_id = 19122232;

	RedisAssetAdd("asset1", asset);
	new_asset = RedisAssetGet("asset1");

	res = AGAssetFree(asset);
	if(res) {
		DEBUG_PRINT("Memory free error.\n",0);
		exit(1);
	}

	res = AGAssetFree(new_asset);
	if(res) {
		DEBUG_PRINT("There was a problem.\n",0);
		exit(1);
	}

	return 0;
}

int RedisExploit_Test()
{
	struct AGExploit *exploit;
	struct AGExploit *new_exploit;

	exploit = calloc(1, sizeof(struct AGExploit));
	if(!exploit) {
		DEBUG_PRINT("Memory allocation error.\n",0);
		exit(1);
	}

	exploit->id = 143222;
	exploit->name = dynstr("exploit1");

	RedisExploitAdd("exploit", exploit);
	new_exploit = RedisExploitGet("exploit");

	AGExploitFree(exploit);
	AGExploitFree(new_exploit);

	return 0;
}

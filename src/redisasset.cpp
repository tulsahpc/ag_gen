/** \file ag_redisasset.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 *
 * Handles the asset to redis server interchange.
 *
 * Takes an assets and turns it into a string that can be put into the redis server.
 * Retrives data from a redis server list and turns it into an asset that can be returned.
 */

#include <iostream>
#include <string>

#include "asset.hpp"
#include "redisasset.hpp"
#include "util_redis.hpp"
#include "util_common.hpp"

using namespace std;

/**
 * Takes in an asset and key and adds the assets data into the redis list given by key.
 *
 * Will print to the console and return 1 if the asset is null.
 */
int rasset_set(const char *key, Asset *asset)
{
	if(asset == NULL) {
		cout << "Asset cannot be null." << endl;
		return 1;
	}

	string redis_query = to_string(asset->get_network_id()) + ":" + asset->get_name() + ":" + to_string(asset->get_network_id());

	redis_enqueue(key, redis_query.c_str());

	return 0;
}

/**
 * Gets the asset out of a redis list given by key.
 *
 * If the list given by key is empty then then it will return a null asset
 */
Asset rasset_get(const char *key)
{
	string redis_value = redis_dequeue(key);
	vector<string> redis_parsed = split(redis_value, ':');

	int asset_id = stoi(redis_parsed[0]);
	string asset_name = redis_parsed[1];
	int asset_network_id = stoi(redis_parsed[2]);

	Asset asset(asset_id, asset_network_id, asset_name);
	return asset;
}

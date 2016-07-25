/** \file ag_facts.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include "ag_asset.h"
#include "ag_facts.h"
#include "util_hash.h"

struct AGQuality *AGQualityNew(struct AGAsset *asset)
{
	struct AGQuality *quality = malloc(sizeof(struct AGQuality));
	quality->asset_id = asset->id;
	quality->hash = HashTableNew();
	return quality;
}

void AGSetProperty(struct AGQuality *quality, enum Property prop, char *val)
{
	struct HashTable *table = quality->hash;
	HashTableAdd(table, property_strings[prop], val);
}

char *AGGetProperty(struct AGQuality *quality, enum Property prop)
{
	return HashTableFind(quality->hash, property_strings[prop]);
}


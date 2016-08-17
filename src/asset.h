/** \file ag_asset.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef ASSET_H
#define ASSET_H

#include "util_list.h"

struct Asset {
	int id;
	char *name;
	int network_id;
	struct HashTable *facts;
};

struct AssetBinding {
	int len;
	struct Asset *assets;
};

int assets_fetch(struct List *, const char *);
struct Asset *asset_new(int, char *);
void asset_print(struct Asset *);
void asset_free(struct Asset *);

#endif //ASSET_H

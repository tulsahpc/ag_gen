/** \file ag_asset.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "ag_asset.h"
#include "db_util.h"

struct AGAssetList *AGGetAssets()
{
	PGresult *res;
	int numRows;
	struct AGAsset **assets;
	struct AGAssetList *assetList;

	AGDbBeginTransaction();

	res = PQexec(conn, "SELECT * FROM asset");
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}

	numRows = PQntuples(res);
	assets = malloc(sizeof(struct AGAsset*) * numRows);

	for (int i=0; i<numRows; i++) {
		char *idValue = PQgetvalue(res, i, 0);
		char *nameValue = PQgetvalue(res, i, 1);
		char *networkIdValue = PQgetvalue(res, i, 2);
		size_t nameLen = strlen(nameValue);
		char *name = calloc(nameLen + 1, sizeof(char));
		strncpy(name, nameValue, nameLen);

		assets[i] = malloc(sizeof(struct AGAsset));
		assets[i]->id = atoi(idValue);
		assets[i]->name = name;
		assets[i]->network_id = atoi(networkIdValue);
	}

	AGDbEndTransaction();
	PQclear(res);

	assetList = malloc(sizeof(struct AGAssetList));
	assetList->assets = assets;
	assetList->len = numRows;

	return assetList;
}

int AGAssetsFree(struct AGAssetList *assetList)
{
	int len = assetList->len;
	struct AGAsset **assets = assetList->assets;

	for(int i=0; i<len; i++) {
		free(assets[i]);
	}

	free(assets);
	return 0;
}

int AGAssetFree(struct AGAsset *asset)
{
	if(asset == NULL) {
		return 1;
	}

	free(asset);
	return 0;
}

void AGAssetsPrint(const struct AGAssetList *assetList)
{
	int len = assetList->len;
	struct AGAsset **assets = assetList->assets;

	for(int i=0; i<len; i++) {
		const struct AGAsset *asset = assets[i];
		printf("Asset %d: %s on Network %d\n",
			asset->id, asset->name, asset->network_id);
	}
}

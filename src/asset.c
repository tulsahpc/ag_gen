/** \file ag_asset.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "asset.h"
#include "util_common.h"
#include "util_list.h"
#include "util_db.h"

#define STRBUF 128

struct List *assets_fetch(const char *network)
{
	PGresult *res;
	int num_rows;
	struct List *asset_list = list_new();

	dbtrans_begin();

	char sql[STRBUF+1] = {0};
	// int wr =
	snprintf(sql, STRBUF, "SELECT * FROM asset WHERE network_id = (SELECT id FROM network WHERE name = '%s');\n", network);
	// if(wr < STRBUF) {
	// 	sql[wr] = '\0';
	// } else {
	// 	sql[STRBUF] = '\0';
	// }

	res = PQexec(conn, sql);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}

	num_rows = PQntuples(res);
	for (int i=0; i<num_rows; i++) {
		int id = atoi(PQgetvalue(res, i, 0));
		char *name = dynstr(PQgetvalue(res, i, 1));
		int network_id = atoi(PQgetvalue(res, i, 2));

		struct Asset *asset = calloc(1, sizeof(struct Asset));
		asset->id = id;
		asset->name = name;
		asset->network_id = network_id;

		list_push(asset_list, asset);
	}

	dbtrans_end();
	PQclear(res);

	return asset_list;
}

struct Asset *asset_new(int id, char *name)
{
	struct Asset *new_asset = calloc(1, sizeof(struct Asset));
	new_asset->id = id;
	new_asset->name = dynstr(name);

	return new_asset;
}

int asset_free(struct Asset *asset)
{
	// DEBUG_PRINT("asset #%d: %s\n", asset->id, asset->name);
	if(asset == NULL)
		return 1;

	if(asset->name != NULL)
		free(asset->name);

	free(asset);
	return 0;
}

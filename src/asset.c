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

int assets_fetch(struct List *asset_list, const char *network)
{
	PGresult *res;
	int num_rows;

	dbtrans_begin();

	char sql[STRBUF+1] = {0};
	snprintf(sql, STRBUF, "SELECT * FROM asset WHERE network_id = (SELECT id FROM network WHERE name = '%s');\n", network);

	res = PQexec(conn, sql);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		goto fatal;
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
	return list_size(asset_list);
fatal:
	PQclear(res);
	for(int i=0; i<list_size(asset_list); i++)
		asset_free(list_at(asset_list, i));
	return -1;
}

struct Asset *asset_new(int id, char *name)
{
	struct Asset *new_asset = calloc(1, sizeof(struct Asset));
	new_asset->id = id;
	new_asset->name = dynstr(name);

	return new_asset;
}

void asset_print(struct Asset *asset)
{
	printf("%s\n", asset->name);
}

void asset_free(struct Asset *asset)
{
	// DEBUG_PRINT("asset #%d: %s\n", asset->id, asset->name);
	free(asset->name);
	free(asset);
}

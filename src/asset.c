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
#include "util_hash.h"

#define STRBUF 128

static int qualities_fetch(struct List *quality_list, int asset_id)
{
	PGresult *res;
	int num_rows;

	char sql[STRBUF+1] = {0};
	snprintf(sql, STRBUF, "SELECT * FROM quality WHERE asset_id = %d;\n", asset_id);

	res = PQexec(conn, sql);
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		goto fatal;
	}

	num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		int asset_id = atoi(PQgetvalue(res, i, 0));
		char *property = dynstr(PQgetvalue(res, i, 1));
		char *value = dynstr(PQgetvalue(res, i, 2));

		struct Quality *qual = malloc(sizeof(struct Quality));
		qual->asset_id = asset_id;
		qual->property = property;
		qual->value = value;

		list_push(quality_list, qual);
	}

	PQclear(res);
	return list_size(quality_list);
fatal:
	PQclear(res);
	return -1;
}

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

		struct HashTable *ht = hashtable_new();
		asset->facts = ht;

		struct List *quality_list = list_new();
		qualities_fetch(quality_list, id);

		for(int j=0; j<list_size(quality_list); j++) {
			struct Quality *next_quality = list_at(quality_list, j);

			char *property = next_quality->property;
			char *value = next_quality->value;

			hashtable_set(ht, dynstr(property), dynstr(value));
		}

		list_iterate(quality_list, quality_free);
		list_free(quality_list);

		list_push(asset_list, asset);
	}

	dbtrans_end();
	PQclear(res);
	return list_size(asset_list);
fatal:
	PQclear(res);
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
	hashtable_free(asset->facts);
	free(asset->name);
	free(asset);
}

void quality_free(struct Quality *qual)
{
	free(qual->property);
	free(qual->value);
	free(qual);
}

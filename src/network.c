/** \file ag_network.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libpq-fe.h>

#include "network.h"
#include "util_common.h"
#include "util_db.h"
#include "util_list.h"

struct List *networks_fetch()
{
	PGresult *res;
	int numRows;
	struct List *network_list;

	dbtrans_begin();

	res = PQexec(conn, "SELECT * FROM network");
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}

	numRows = PQntuples(res);
	network_list = list_new();

	for(int i=0; i<numRows; i++) {
		int id = atoi(PQgetvalue(res, i, 0));
		char *name = dynstr(PQgetvalue(res, i, 1));

		struct Network *new_network = malloc(sizeof(struct Network));
		new_network->id = id;
		new_network->name = name;

		list_push(network_list, new_network);
	}

	dbtrans_end();
	PQclear(res);

	return network_list;
}

int network_free(struct Network *network)
{
	if(network == NULL)
		return -1;

	if(network->name != NULL)
		free(network->name);

	free(network);
	return 0;
}

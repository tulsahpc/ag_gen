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

// Returns
//	int >= 0: number of elements in the list
//	int < 0: error occurred
int networks_fetch(struct List *network_list)
{
	PGresult *res;
	int numRows;

	dbtrans_begin();

	res = PQexec(conn, "SELECT * FROM network");
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
		goto fatal;
	}

	numRows = PQntuples(res);
	for(int i=0; i<numRows; i++) {
		int id = atoi(PQgetvalue(res, i, 0));
		char *name = dynstr(PQgetvalue(res, i, 1));

		struct Network *new_network = malloc(sizeof(struct Network));
		new_network->id = id;
		new_network->name = name;

		int res = list_push(network_list, new_network);
		if(res != 0)
			fprintf(stderr, "Error occurred adding network to list.");
	}

	dbtrans_end();
	PQclear(res);
	return list_size(network_list);
fatal:
	PQclear(res);
	return -1;
}

void network_free(struct Network *network)
{
	if(network == NULL) return;
	free(network->name);
	free(network);
}

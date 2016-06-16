/** \file ag_network.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libpq-fe.h>

#include "ag_network.h"
#include "db_util.h"
#include "util.h"

/**
 *
 *
 *
 */
struct AGNetworkList *AGGetNetworks()
{
	PGresult *res;
	int numRows;
	struct AGNetwork **networks;
	struct AGNetworkList *networkList;

	AGDbBeginTransaction();

	res = PQexec(conn, "SELECT * FROM network");
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely();
	}

	numRows = PQntuples(res);
	networks = malloc(sizeof(struct AGNetwork*) * numRows);

	for(int i=0; i<numRows; i++) {
		int id = atoi(PQgetvalue(res, i, 0));
		char *name = dynstr(PQgetvalue(res, i, 1));

		networks[i] = malloc(sizeof(struct AGNetwork));
		networks[i]->id = id;
		networks[i]->name = name;
	}

	AGDbEndTransaction();
	PQclear(res);

	networkList = malloc(sizeof(struct AGNetworkList));
	networkList->networks = networks;
	networkList->len = numRows;

	return networkList;
}

/**
 *
 *
 *
 */
int AGNetworksFree(struct AGNetworkList *networkList)
{
	int len = networkList->len;
	struct AGNetwork **networks = networkList->networks;

	for(int i=0; i<len; i++) {
		free(networks[i]->name);
		free(networks[i]);
	}

	free(networks);
	return 0;
}

/**
 *
 *
 *
 */
void AGNetworksPrint(const struct AGNetworkList *networkList)
{
	int len = networkList->len;
	struct AGNetwork **networks = networkList->networks;

	for(int i=0; i<len; i++) {
		const struct AGNetwork *network = networks[i];
		printf("Network %d: %s\n", network->id, network->name);
	}
}

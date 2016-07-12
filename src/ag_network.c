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

int AGNetworkFree(struct AGNetwork *network)
{
	if(network == NULL)
		return 1;

	if(network->name != NULL)
		free(network->name);

	free(network);
	return 0;
}

int AGNetworkListFree(struct AGNetworkList *networkList)
{
	struct AGNetwork **networks;
	if(networkList == NULL)
		return 1;

	int len = networkList->len;
	if(networkList->networks != NULL) {
		networks = networkList->networks;
		for(int i=0; i<len; i++) {
			AGNetworkFree(networks[i]);
		}

		free(networks);
	}

	free(networkList);
	return 0;
}

int AGNetworkListPrint(const struct AGNetworkList *networkList)
{
	if(networkList == NULL)
		return 1;

	int len = networkList->len;
	struct AGNetwork **networks = networkList->networks;
	if(networks != NULL) {
		for(int i=0; i<len; i++) {
			const struct AGNetwork *network = networks[i];
			if(network != NULL)
				printf("Network %d: %s\n", network->id, network->name);
		}
	}

	return 0;
}

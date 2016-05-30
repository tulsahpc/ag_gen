#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libpq-fe.h>

#include "ag_network.h"
#include "db_util.h"

#define BUFSIZE 32

struct AGNetworkList *AGGetNetworks() {
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
        char *idValue;
        char *nameValue;
        char *name;
        size_t nameLen;

        idValue = PQgetvalue(res, i, 0);
        nameValue = PQgetvalue(res, i, 1);
        nameLen = strlen(nameValue);
        name = calloc(nameLen+1, sizeof(char));

        strncpy(name, nameValue, nameLen);

        networks[i] = malloc(sizeof(struct AGNetwork));
        networks[i]->id = atoi(idValue);
        networks[i]->name = name;
    }

    AGDbEndTransaction();
    PQclear(res);

    networkList = malloc(sizeof(struct AGNetworkList));
    networkList->networks = networks;
    networkList->len = numRows;

    return networkList;
}

int AGNetworksFree(struct AGNetworkList *networkList) {
    int len;
    struct AGNetwork **networks;

    len = networkList->len;
    networks = networkList->networks;

    for(int i=0; i<len; i++) {
        free(networks[i]->name);
        free(networks[i]);
    }

    free(networks);
    return 0;
}

void AGNetworksPrint(const struct AGNetworkList *networkList) {
    int len;
    struct AGNetwork **networks;

    len = networkList->len;
    networks = networkList->networks;

    for(int i=0; i<len; i++) {
        const struct AGNetwork *network;

        network = networks[i];
        printf("Network %d: %s\n", network->id, network->name);
    }
}

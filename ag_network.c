#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "ag_network.h"
#include "db_util.h"

struct AGNetworkList *AGGetNetworks() {
    PGresult *res;

    AGDbBeginTransaction();

    res = PQexec(conn, "SELECT * FROM network");
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely();
    }

    int numRows = PQntuples(res);
    struct AGNetwork **networks = malloc(sizeof(struct AGNetwork*) * numRows);

    for(int i=0; i<numRows; i++) {
        char *idValue = PQgetvalue(res, i, 0);
        char *nameValue = PQgetvalue(res, i, 1);
        size_t nameLen = strlen(nameValue);

        char *name = calloc(nameLen+1, sizeof(char));
        strncpy(name, nameValue, nameLen);

        networks[i] = malloc(sizeof(struct AGNetwork));
        networks[i]->id = atoi(idValue);
        networks[i]->name = name;
    }

    AGDbEndTransaction();
    PQclear(res);

    struct AGNetworkList *networkList = malloc(sizeof(struct AGNetworkList));
    networkList->networks = networks;
    networkList->len = numRows;

    return networkList;
}

int AGNetworksFree(struct AGNetworkList *networkList) {
    int len = networkList->len;
    struct AGNetwork **networks = networkList->networks;

    for(int i=0; i<len; i++) {
        free(networks[i]->name);
        free(networks[i]);
    }

    free(networks);
    return 0;
}

void AGNetworksPrint(const struct AGNetworkList *networkList) {
    int len = networkList->len;
    struct AGNetwork **networks = networkList->networks;

    for(int i=0; i<len; i++) {
        const struct AGNetwork *network = networks[i];
        printf("Network %d: %s\n", network->id, network->name);
    }
}

#include <vector>
#include <string>

#include <libpq-fe.h>
#include "network.h"
#include "util_db.h"

using namespace std;

int networks_fetch(vector<Network *> &network_list)
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
		int id = stoi(PQgetvalue(res, i, 0));
		string name = PQgetvalue(res, i, 1);

		Network *new_network = new Network;
		new_network->id = id;
		new_network->name = name;

		network_list.push_back(new_network);
	}

	dbtrans_end();
	PQclear(res);
	return network_list.size();
fatal:
	PQclear(res);
	return -1;
}

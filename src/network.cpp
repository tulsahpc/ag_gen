#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include <libpq-fe.h>
#include "network.h"
#include "util_db.h"

using namespace std;

int networks_fetch(vector<shared_ptr<Network> > &network_list)
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

		shared_ptr<Network> network(new Network);
		network->id = id;
		network->name = name;

		network_list.push_back(network);
	}

	dbtrans_end();
	PQclear(res);
	return network_list.size();
fatal:
	PQclear(res);
	return -1;
}

shared_ptr<Network> find_network(const string name)
{
	PGresult *res;
	int numRows;
	shared_ptr<Network> network(new Network);

	dbtrans_begin();

	res = PQexec(conn, ("SELECT * FROM network where network.name = '" + name + "';").c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
		goto fatal;
	}

	numRows = PQntuples(res);
	if(numRows < 1) {
		throw runtime_error("Network not found.");
	} else {
		int id = stoi(PQgetvalue(res, 0, 0));
		string name = PQgetvalue(res, 0, 1);

		network->id = id;
		network->name = name;
	}

	dbtrans_end();
	PQclear(res);
	return network;
fatal:
	PQclear(res);
	return nullptr;
}

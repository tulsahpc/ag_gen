#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include <libpq-fe.h>

#include "network.hpp"
#include "util_db.hpp"

using namespace std;

int Network::fetch_all(vector<shared_ptr<Network> > &network_list)
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

shared_ptr<Network> Network::find(const string net)
{
	PGresult *res;

	dbtrans_begin();

	res = PQexec(conn, ("SELECT * FROM network where network.name = '" + net + "';").c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		PQclear(res);
		dbtrans_end();
		throw runtime_error("There was a db error.");
	}

	auto numRows = PQntuples(res);
	if(numRows == 0) {
		PQclear(res);
		dbtrans_end();
		throw runtime_error("Network not found.");
	}

	shared_ptr<Network> network(new Network);
	int id = stoi(PQgetvalue(res, 0, 0));
	string name = PQgetvalue(res, 0, 1);

	network->id = id;
	network->name = name;

	dbtrans_end();
	PQclear(res);

	return network;
}

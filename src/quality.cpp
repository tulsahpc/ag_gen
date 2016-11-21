#include <iostream>
#include <memory>
#include <vector>

#include <libpq-fe.h>
#include "fact.hpp"
#include "util_db.h"

using namespace std;

Quality::Quality(int asset, string const& qualName, string const& qualValue) : asset_id(asset), name(qualName), value(qualValue) {}

int Quality::fetch(int asset_id) {
	PGresult *res;
	int num_rows;

	string sql = "SELECT * FROM quality WHERE asset_id = '" + to_string(asset_id) + "';";

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
	}

	vector<shared_ptr<Quality> > quality_list;

	num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		int asset_id = stoi(PQgetvalue(res, i, 0));
		string property = PQgetvalue(res, i, 1);
		string value = PQgetvalue(res, i, 2);

		shared_ptr<Quality> qual(new Quality(asset_id, property, value));

		quality_list.push_back(qual);
	}

	PQclear(res);
	return quality_list.size();
}

void Quality::print() {
    std::cout << name + ": " + value << std::endl;
}

#include <vector>
#include <string>
#include <libpq-fe.h>

#include "asset.h"
#include "quality.h"
#include "util_db.h"

using namespace std;

Asset::Asset(int id, int network_id, string asset_name) : id(id), network_id(network_id), name(asset_name) {};

int Asset::get_network_id(void) {
	return network_id;
}

string Asset::get_name(void) {
	return name;
}

void Asset::fetch_qualities(void)
{
	vector<Quality> qualities;

	PGresult *res;
	int num_rows;

	string sql = "SELECT * FROM quality WHERE asset_id = '" + to_string(id) + "';";

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		PQclear(res);
		exit(1);
	}

	num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		int asset_id = stoi(PQgetvalue(res, i, 0));
		string property = PQgetvalue(res, i, 1);
		string value = PQgetvalue(res, i, 2);

		Quality qual(asset_id, property, value);
		qualities.push_back(qual);
	}

	PQclear(res);
	this->qualities = qualities;
}

vector<Asset> Asset::fetch_all(const string& network)
{
	vector<Asset> assets;

	PGresult *res;
	int num_rows;

	dbtrans_begin();

	string sql = "SELECT * FROM asset WHERE network_id = (SELECT id FROM network WHERE name = '" + network + "');";

	res = PQexec(conn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		PQclear(res);
		exit(1);
	}

	num_rows = PQntuples(res);
	for (int i=0; i<num_rows; i++) {
		int id = stoi(PQgetvalue(res, i, 0));
		string name = PQgetvalue(res, i, 1);
		int network_id = stoi(PQgetvalue(res, i, 2));

		Asset asset(id, network_id, name);
		asset.fetch_qualities();

		assets.push_back(asset);
	}

	dbtrans_end();
	PQclear(res);
	return assets;
}

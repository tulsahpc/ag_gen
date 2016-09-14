#include <vector>
#include <memory>
#include <string>
#include <libpq-fe.h>
#include "asset.h"
#include "util_db.h"

using namespace std;

static int qualities_fetch(vector<shared_ptr<Quality> > &quality_list, int asset_id)
{
	PGresult *res;
	int num_rows;

	string sql = "SELECT * FROM quality WHERE asset_id = '" + to_string(asset_id) + "';";

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		goto fatal;
	}

	num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		int asset_id = stoi(PQgetvalue(res, i, 0));
		string property = PQgetvalue(res, i, 1);
		string value = PQgetvalue(res, i, 2);

		shared_ptr<Quality> qual(new Quality);
		qual->asset_id = asset_id;
		qual->property = property;
		qual->value = value;

		quality_list.push_back(qual);
	}

	PQclear(res);
	return quality_list.size();
fatal:
	PQclear(res);
	return -1;
}

int assets_fetch(vector<shared_ptr<Asset> > &asset_list, string const &network)
{
	PGresult *res;
	int num_rows;

	dbtrans_begin();

	string sql = "SELECT * FROM asset WHERE network_id = (SELECT id FROM network WHERE name = '" + network + "');";

	res = PQexec(conn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
		goto fatal;
	}

	num_rows = PQntuples(res);
	for (int i=0; i<num_rows; i++) {
		int id = stoi(PQgetvalue(res, i, 0));
		auto name = PQgetvalue(res, i, 1);
		int network_id = stoi(PQgetvalue(res, i, 2));

		shared_ptr<Asset> asset(new Asset);
		asset->id = id;
		asset->name = name;
		asset->network_id = network_id;

		vector<shared_ptr<Quality> > quality_list;
		qualities_fetch(quality_list, id);

		asset_list.push_back(asset);
	}

	dbtrans_end();
	PQclear(res);
	return asset_list.size();
fatal:
	PQclear(res);
	return -1;
}

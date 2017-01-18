#include <iostream>
#include <memory>
#include <vector>
#include <libpq-fe.h>

#include "ag_gen.hpp"
#include "global.hpp"
#include "fact.hpp"
#include "util_db.hpp"

using namespace std;

Quality::Quality(int asset, string qualName, string qualValue) : asset_id(asset), name(qualName), value(qualValue) {}

string Quality::get_name() {
	return name;
}

int Quality::encoded() {
	EncodedQuality qual;
	// qual.dec.asset_id = asset_id;
	// qual.dec.attr = generator.attrs[name];
	// qual.dec.val = generator.vals[value];
	return qual.enc;
}

void Quality::print() {
    std::cout << name + ": " + value << std::endl;
}

vector<Quality> Quality::fetch_all() {
	vector<Quality> qualities;

	PGresult *res;
	int num_rows;

	string sql = "SELECT * FROM quality;";

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
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
	return qualities;
}

vector<string> Quality::fetch_all_attributes() {
	vector<string> attrs;

	PGresult *res;

	string sql = "SELECT DISTINCT property FROM quality;";
	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
	}

	int num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		string prop = PQgetvalue(res, i, 0);
		attrs.push_back(prop);
	}

	PQclear(res);
	return attrs;
}

vector<string> Quality::fetch_all_values() {
	vector<string> vals;

	PGresult *res;

	string sql = "SELECT DISTINCT value FROM quality;";
	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT command failed: %s",
			PQerrorMessage(conn));
	}

	int num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		string val = PQgetvalue(res, i, 0);
		vals.push_back(val);
	}

	PQclear(res);
	return vals;
}

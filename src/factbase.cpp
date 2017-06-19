// factbase.cpp implements the Factbase class which contains the known facts that are completely true in a
// particular Network State

#include <iostream>
#include <algorithm>
#include <vector>

#include "factbase.h"
#include "util_db.h"

using namespace std;

// The default Factbase constructor creates a factbase object with all of the qualities and topologies
// currently on the database
Factbase::Factbase(void) {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

Factbase::Factbase(const Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {}

// find_quality searches for a given quality in a factbase. Returns true if the quality is found, otherwise
// returns false
bool Factbase::find_quality(const Quality& q) const {
    if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
		return false;
    }
    return true;
}

// find_topology searches for a given topology in a factbase. Returns true if the topology is found,
// otherwise returns false
bool Factbase::find_topology(const Topology& t) const {
    if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

// add_quality adds a given quality to the factbase's vector of qualities
void Factbase::add_quality(const Quality& q) {
    qualities.push_back(q);
}

// add_topology adds a given topology to the factbase's vector of topologies
void Factbase::add_topology(const Topology& t) {
    topologies.push_back(t);
}

void Factbase::save(void) {
	PGresult *res;
	int num_rows;

	string sql = "SELECT * FROM quality;";

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "quality SELECT command failed: %s",
				PQerrorMessage(conn));
	}

	num_rows = PQntuples(res);
	for(int i=0; i<num_rows; i++) {
		int asset_id = stoi(PQgetvalue(res, i, 0));
		string property = PQgetvalue(res, i, 1);
		string value = PQgetvalue(res, i, 2);

		const Quality qual(asset_id, property, value);
		qualities.push_back(qual);
	}

	PQclear(res);
}

// print prints to stdout every quality of the factbase, then every topology of the factbase
void Factbase::print(void) const {
    for(auto& q : qualities) {
        q.print();
    }

    for(auto& t : topologies) {
        t.print();
    }
}

size_t Factbase::hash(const Factbase& fb) {
    auto hash = FactbaseHash{}(fb);
    return hash;
}

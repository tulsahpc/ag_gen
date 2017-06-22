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

int Factbase::request_id(void) {
	PGresult *res;
	string sql = "SELECT new_factbase();";

	dbtrans_begin();

	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "new_factbase() SELECT command failed: %s",
				PQerrorMessage(conn));
	}
	int factbase_id = stoi(PQgetvalue(res, 0, 0));

	dbtrans_end();
	PQclear(res);

	return factbase_id;
}

void Factbase::save(void) {
	PGresult *res;

	int id = request_id();
	this->id = id;

    dbtrans_begin();

    // XXX: There has to be a better way to do this
    string insert_sql = "INSERT INTO factbase_item VALUES ";
    insert_sql += "(" + to_string(this->id) + "," + to_string(qualities[0].encode().enc) + ")";
    for(int i=1; i<qualities.size(); i++) {
        insert_sql += ",(" + to_string(this->id) + "," + to_string(qualities[i].encode().enc) + ")";
    }
	for(int i=0; i<topologies.size(); i++) {
		insert_sql += ",(" + to_string(this->id) + "," + to_string(topologies[i].encode().enc) + ")";
	}
    insert_sql += ";";

    res = PQexec(conn, insert_sql.c_str());
    if(PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "factbase_item INSERT command failed: %s",
                PQerrorMessage(conn));
    }

    dbtrans_end();

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

size_t Factbase::hash(void) {
    auto hash = FactbaseHash{}(*this);
	this->hash_value = hash;
    return hash;
}

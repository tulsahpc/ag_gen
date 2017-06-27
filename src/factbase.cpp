// factbase.cpp implements the Factbase class which contains the known facts that are completely true in a
// particular Network State

#include <iostream>
#include <vector>

#include "factbase.h"
#include "util_db.h"

using namespace std;

// The default Factbase constructor creates a factbase object with all of the qualities and topologies
// currently on the database
Factbase::Factbase(void) : qualities(), topologies() {
	id = 0;
}

Factbase::Factbase(Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {
    id = 0;
}

Factbase::Factbase(int iId) {
	PGresult *res;

	// Check if factbase exists
	// If it does exist, import all of its data
	// If it doesn't exist, throw exception

	string sql = "SELECT * FROM factbase WHERE id = " + to_string(id) + ";";
	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT factbase error: %s", PQerrorMessage(conn));
	}

	id = iId;
	size_t hash_value;
	sscanf(PQgetvalue(res, 0, 0), "%zu", &hash_value);

	sql = "SELECT * FROM factbase_item WHERE factbase_id = " + to_string(id) + ";";
	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		cerr << "SELECT factbase_items failed: " << PQerrorMessage(conn) << endl;
	}

	int num_rows = PQntuples(res);
	for(auto i=0; i<num_rows; i++) {
		size_t fact;
		sscanf(PQgetvalue(res, i, 1), "%zu", &fact);

		string type = PQgetvalue(res, i, 2);
		if(type == "quality") {
			Quality qual(fact);
            add_quality(qual);
		}

        if(type == "topology") {
            string options = PQgetvalue(res, i, 3);
            Topology topo(fact, options);
            add_topology(topo);
        }
	}
}

int Factbase::get_id(void) {
	if(id == 0) {
		PGresult *res;
		string sql = "SELECT id FROM factbase WHERE hash = '" + to_string(hash()) + "';";
		res = PQexec(conn, sql.c_str());

		// If has of current factbase exists, pull that id
		int numrows = PQntuples(res);
		if(numrows > 0) {
			id = stoi(PQgetvalue(res, 0, 0));
			return id;
		} else { // Else, get a new id from the db
			sql = "SELECT new_factbase();";

			res = PQexec(conn, sql.c_str());
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				fprintf(stderr, "new_factbase() SELECT command failed: %s",
						PQerrorMessage(conn));
			}
			int factbase_id = stoi(PQgetvalue(res, 0, 0));
			id = factbase_id;

			PQclear(res);
			return factbase_id;
		}
	} else {
		return id;
	}
}

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
	get_id();

    dbtrans_begin();

	// Save hash
	string hash_sql = "UPDATE factbase SET hash = '" + to_string(this->hash()) + "' WHERE id = " + to_string(id) + ";";
	res = PQexec(conn, hash_sql.c_str());
	if(PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "factbase UPDATE hash command failed: %s",
			PQerrorMessage(conn));
	}

    // XXX: There has to be a better way to do this
    string insert_sql = "INSERT INTO factbase_item VALUES ";
    insert_sql += "(" + to_string(id) + "," + to_string(qualities[0].encode().enc) + ",'quality', NULL)";
    for(int i=1; i<qualities.size(); i++) {
        insert_sql += ",(" + to_string(id) + "," + to_string(qualities[i].encode().enc) + ",'quality', NULL)";
    }
	for(int i=0; i<topologies.size(); i++) {
		insert_sql += ",(" + to_string(id) + "," + to_string(topologies[i].encode().enc) + ",'topology','" + topologies[i].get_raw_options() + "')";
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

size_t Factbase::hash(void) const {
    auto hash = FactbaseHash{}(*this);
    return hash;
}

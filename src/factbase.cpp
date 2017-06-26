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
	id = 0;
	hash_value = 0;
}

Factbase::Factbase(const Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {
    id = 0;
}

Factbase::Factbase(int id) {
	PGresult *res;

	dbtrans_begin();

	string sql = "SELECT * FROM factbase WHERE id = " + to_string(id) + ";";
	res = PQexec(conn, sql.c_str());
	if(PQresultStatus(res) != PGRES_TUPLES_OK) {
		fprintf(stderr, "SELECT factbase error: %s", PQerrorMessage(conn));
	}

	Factbase new_factbase;
	new_factbase.id = id;
	sscanf(PQgetvalue(res, 0, 0), "%zu", &new_factbase.hash_value);

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
            new_factbase.add_quality(qual);
		}

        if(type == "topology") {
            string options = PQgetvalue(res, i, 3);
            Topology topo(fact, options);
            new_factbase.add_topology(topo);
        }
	}

	dbtrans_end();
}

int Factbase::get_id(void) const {
	if(id == 0) {
		return -1;
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

int Factbase::request_id(void) {
	if(this->id == 0) {
		PGresult *res;
		string sql = "SELECT new_factbase();";

		dbtrans_begin();

		res = PQexec(conn, sql.c_str());
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			fprintf(stderr, "new_factbase() SELECT command failed: %s",
					PQerrorMessage(conn));
		}
		int factbase_id = stoi(PQgetvalue(res, 0, 0));

		dbtrans_end();
		PQclear(res);

		this->id = factbase_id;

		return factbase_id;
	} else {
		return this->id;
	}
}

void Factbase::save(void) {
	PGresult *res;

    // Check if I exist already
    cout << "hash: " + hash_value << endl;
    cout << "id: " + id << endl;
    if(hash_value != 0) {
        string exist_sql = "SELECT * FROM factbase WHERE hash = '" + to_string(hash_value) + "';";
        PGresult *res = PQexec(conn, exist_sql.c_str());
        int numrows = PQntuples(res);
        cout << numrows << endl;
        if(numrows != 0) {
            // Factbase already exists in database
            return;
        }
    }

	if(id == 0) {
		id = this->request_id();
	}

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

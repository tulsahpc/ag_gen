// factbase.cpp implements the Factbase class which contains the known facts that are completely true in a
// particular Network State

#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

#include "factbase.h"
#include "util_db.h"


using namespace std;

// The default Factbase constructor creates a factbase object with all of the qualities and topologies
// currently on the database
Factbase::Factbase(void) : qualities(), topologies() {
	id = 0;
}

Factbase::Factbase(const Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {
    id = 0;
}

Factbase::Factbase(int iId) {
	// Check if factbase exists
	// If it does exist, import all of its data
	// If it doesn't exist, throw exception

	vector<DB::Row> rows = db->exec("SELECT * FROM factbase WHERE id = " + to_string(iId) + ";");
	if(rows.size() != 1) {
		throw DBException("Something went wrong.");
	}

	// There should only be one row that is returned,
	// so shortcut with rows[0][0]
	size_t hash_value;
	sscanf(rows[0][0].c_str(), "%zu", &hash_value);

	rows = db->exec("SELECT * FROM factbase_item WHERE factbase_id = " + to_string(iId) + ";");

	for(auto& row : rows) {
		size_t fact;
		sscanf(row[1].c_str(), "%zu", &fact);

		string type = row[2];
		if(type == "quality") {
			Quality qual(fact);
            add_quality(qual);
		}

        if(type == "topology") {
            string options = row[3];
            Topology topo(fact, options);
            add_topology(topo);
        }
	}
}

void Factbase::populate() {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

int Factbase::new_id(void) {
	size_t newhash = hash();
	// Should only be one result.
	if(exists_in_db()) {
		vector<DB::Row> rows = db->exec("SELECT id FROM factbase WHERE hash = '" + to_string(newhash) + "';");
		id = stoi(rows[0][0]);
		return id;
	} else { // Else, get a new id from the db
		vector<DB::Row> rows = db->exec("SELECT new_factbase('" + to_string(newhash) + "');");
		int factbase_id = stoi(rows[0][0]);
		id = factbase_id;
		return factbase_id;
	}
}

int Factbase::get_id(void) {
	if(id != 0) {
		return id;
	} else {
		return new_id();
	}
}

bool Factbase::exists_in_db(void) {
	vector<DB::Row> rows = db->exec("SELECT 1 FROM factbase WHERE hash = '" + to_string(hash()) + "';");
	if(rows.size() > 0) {
		return true;
	} else {
		return false;
	}
}

// find_quality searches for a given quality in a factbase. Returns true if the quality is found, otherwise
// returns false
bool Factbase::find_quality(Quality& q) const {
    if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
		return false;
    }
    return true;
}

// find_topology searches for a given topology in a factbase. Returns true if the topology is found,
// otherwise returns false
bool Factbase::find_topology(Topology& t) const {
    if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

// add_quality adds a given quality to the factbase's vector of qualities
void Factbase::add_quality(Quality& q) {
    qualities.push_back(q);
}

// add_topology adds a given topology to the factbase's vector of topologies
void Factbase::add_topology(Topology& t) {
    topologies.push_back(t);
}

void Factbase::save(void) {
	int myid = get_id();

    // XXX: There has to be a better way to do this
    string insert_sql = "INSERT INTO factbase_item VALUES ";
    insert_sql += "(" + to_string(myid) + "," + to_string(qualities[0].encode().enc) + ",'quality', NULL)";
    for(int i=1; i<qualities.size(); i++) {
        insert_sql += ",(" + to_string(myid) + "," + to_string(qualities[i].encode().enc) + ",'quality', NULL)";
    }
	for(int i=0; i<topologies.size(); i++) {
		insert_sql += ",(" + to_string(myid) + "," + to_string(topologies[i].encode().enc) + ",'topology','" + topologies[i].get_raw_options() + "')";
	}
    insert_sql += " ON CONFLICT DO NOTHING;";

    db->exec(insert_sql);
}

size_t Factbase::hash(void) {
    auto hash = FactbaseHash{}(*this);
    return hash;
}

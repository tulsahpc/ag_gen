// factbase.cpp implements the Factbase class which contains the known facts that are completely true in a
// particular Network State

#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

#include "factbase.h"
#include "util_db.h"
#include "util_common.h"

using namespace std;

// The default Factbase constructor creates a factbase object with all of the qualities and topologies
// currently on the database
Factbase::Factbase() : qualities(), topologies() {
	id = 0;
}

Factbase::Factbase(const Factbase& fb) : qualities(), topologies() {
    id = 0;
    copy(fb.qualities.begin(), fb.qualities.end(), back_inserter(qualities));
    copy(fb.topologies.begin(), fb.topologies.end(), back_inserter(topologies));
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
            Topology topo(fact);
            add_topology(topo);
        }
	}
}

void Factbase::populate() {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

int Factbase::get_id() const {
	return id;
}

bool Factbase::exists_in_db() {
    string sql = "SELECT 1 FROM factbase WHERE hash = '" + to_string(hash()) + "';";
	vector<DB::Row> rows = db->exec(sql);
	if(rows.size() > 0) {
        id = stoi(rows[0][0]);
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
void Factbase::add_quality(Quality q) {
    qualities.push_back(q);
}

// add_topology adds a given topology to the factbase's vector of topologies
void Factbase::add_topology(Topology t) {
    topologies.push_back(t);
}

void Factbase::save() {
    if(exists_in_db()) {
        return;
    }

    vector<DB::Row> rows = db->exec("SELECT new_factbase('" + to_string(hash()) + "');");
    id = stoi(rows[0][0]);

    // XXX: There has to be a better way to do this
    string insert_sql = "INSERT INTO factbase_item VALUES ";
    insert_sql += "(" + to_string(id) + "," + to_string(qualities[0].encode().enc) + ",'quality')";
    for(int i=1; i<qualities.size(); i++) {
        insert_sql += ",(" + to_string(id) + "," + to_string(qualities[i].encode().enc) + ",'quality')";
    }
	for(int i=0; i<topologies.size(); i++) {
		insert_sql += ",(" + to_string(id) + "," + to_string(topologies[i].encode().enc) + ",'topology')";
	}
    insert_sql += " ON CONFLICT DO NOTHING;";

    db->exec(insert_sql);
}

// Shamelessly copied from Boost::hash_combine
size_t combine(size_t seed) {
    seed ^= std::hash<size_t>{}(seed) +
            0x9e3779b97f4a7c15 +
            (seed << 6) +
            (seed >> 2);
    return seed;
}

size_t Factbase::hash() const {
    //    size_t hash = 0xf848b64e; // Random seed value
    size_t hash = 0x0c32a12fe19d2119;
    for(auto qual : qualities) {
        EncodedQuality encoded = qual.encode();
        hash ^= combine(encoded.enc);
    }
    for(auto topo : topologies) {
        EncodedTopology encoded = topo.encode();
        hash ^= combine(encoded.enc);
    }
    return hash;
}

void Factbase::print() const {
    cout << "ID: " << id << endl;
    cout << "HASH: " << hash() << endl;
    cout << "Qualities: " << qualities.size() << endl;
    cout << "Topologies: " << topologies.size() << endl << endl;
}

//
// Created by Kyle Cook on 6/24/17.
//

#include <iostream>

#include "util_db.h"
#include "edge.h"

using namespace std;

Edge::Edge(int iFrom, int iTo, Exploit& ex, AssetGroup& ag) : from_node(iFrom), to_node(iTo), exploit(ex), assetGroup(ag) {
	id = 0;
}

int Edge::new_id(void) {
	// Should only be one result.
	if(exists_in_db()) {
		vector<DB::Row> rows = db->exec("SELECT id FROM edge WHERE from_node = " + to_string(from_node) + " AND to_node = " + to_string(to_node) + " AND exploit_id = " + to_string(exploit.get_id()) + ";");
		id = stoi(rows[0][0]);
		return id;
	} else { // Else, get a new id from the db
		vector<DB::Row> rows = db->exec("SELECT new_edge(" + to_string(from_node) + "," + to_string(to_node) + "," + to_string(exploit.get_id()) + ");");
		int factbase_id = stoi(rows[0][0]);
		id = factbase_id;
		return factbase_id;
	}
}

int Edge::get_id(void) {
	if(id != 0) {
		return id;
	} else {
		return new_id();
	}
}

bool Edge::exists_in_db(void) {
	vector<DB::Row> rows = db->exec("SELECT 1 FROM edge WHERE from_node = " + to_string(from_node) + " AND to_node = " + to_string(to_node) + " AND exploit_id = " + to_string(exploit.get_id()) + ";");
	if(rows.size() > 0) {
		return true;
	} else {
		return false;
	}
}

void Edge::save(void) {
	int myid = get_id();

	string sql = "INSERT INTO edge_asset_binding VALUES ";
	for(auto i=0; i<assetGroup.perm.size(); i++) {
		sql += "(" + to_string(myid) + "," + to_string(i) + "," + to_string(assetGroup.perm[i]) + ");";
	}

	db->exec(sql);
}

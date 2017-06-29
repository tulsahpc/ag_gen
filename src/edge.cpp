//
// Created by Kyle Cook on 6/24/17.
//

#include <iostream>

#include "util_db.h"
#include "edge.h"

using namespace std;

bool Edge::exists_in_db(void) {
	vector<DB::Row> rows = db->exec("SELECT 1 FROM edge WHERE from_node = " + to_string(from_node) + " AND to_node = " + to_string(to_node) + ";");
	cout << rows.size() << endl;
	if(rows.size() > 0) {
		return true;
	} else {
		return false;
	}
}

void Edge::save(void) {
    db->exec("INSERT INTO edge VALUES (" + to_string(from_node) + "," + to_string(to_node) + ");");
}

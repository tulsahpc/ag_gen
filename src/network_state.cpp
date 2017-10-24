//
// Created by Kyle Cook on 6/26/17.
//

#include <vector>

#include "network_state.h"
#include "util_db.h"

using namespace std;

Factbase NetworkState::get_factbase() const {
    return factbase;
}

void NetworkState::init_qualities() {
	vector<DB::Row> qrows = db->exec("SELECT DISTINCT property FROM quality;");
	vector<DB::Row> erows = db->exec("SELECT DISTINCT property FROM exploit_postcondition;");
	vector<DB::Row> qvrows = db->exec("SELECT DISTINCT value FROM quality;");
	vector<DB::Row> evrows = db->exec("SELECT DISTINCT value FROM exploit_postcondition;");

	for (auto &row : qrows) {
		string prop = row[0];
		all_attrs.push_back(prop);
	}

	for (auto &row : erows) {
		string prop = row[0];
		all_attrs.push_back(prop);
	}

	for (auto &row : qvrows) {
		string val = row[0];
		all_vals.push_back(val);
	}

	for (auto &row : evrows) {
		string val = row[0];
		all_vals.push_back(val);
	}

	attrs_kv = Keyvalue<string> {all_attrs};
	vals_kv = Keyvalue<string> {all_vals};
}

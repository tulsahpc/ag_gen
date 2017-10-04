#include <iostream>
#include <vector>
#include <libpq-fe.h>

#include "quality.h"
#include "util_db.h"
#include "keyvalue.h"


using namespace std;

Quality::Quality(int asset, string qualName, string op, string qualValue) : asset_id(asset), name(qualName), value(qualValue) {}

Quality::Quality(size_t fact) {
	EncodedQuality eQual;
	eQual.enc = fact;

	asset_id = eQual.dec.asset_id;
	name = eQual.dec.attr;
	op = eQual.dec.op;
	value = eQual.dec.val;
}

string Quality::get_name() {
	return name;
}

string Quality::get_op() {
	return op;
}

EncodedQuality Quality::encode() {
    vector<string> attrs = fetch_all_attributes();
    vector<string> vals = fetch_all_values();

    Keyvalue<string> attrs_kv(attrs);
    Keyvalue<string> vals_kv(vals);

	EncodedQuality qual;
    qual.dec.asset_id = asset_id;
    qual.dec.attr = attrs_kv[name];
    qual.dec.val = vals_kv[value];

//    cout << asset_id << " " << attrs_kv[name] << " " << vals_kv[value] << " - " << qual.enc << endl;
//	cout << attrs.size() << " " << vals.size() << endl;

	return qual;
}

void Quality::print() {
    std::cout << to_string(asset_id) + ": " + name + " => " + value << std::endl;
}

vector<Quality> Quality::fetch_all() {
	vector<Quality> qualities;
	vector<DB::Row> rows = db->exec("SELECT * FROM quality;");

	for(auto row : rows) {
		int asset_id = stoi(row[0]);
		string property = row[1];
		string op = row[2];
		string value = row[3];

		Quality qual(asset_id, property, op, value);
		qualities.push_back(qual);
	}

	return qualities;
}

vector<string> Quality::fetch_all_attributes() {
	vector<string> attrs;
	vector<DB::Row> qrows = db->exec("SELECT DISTINCT property FROM quality;");
    vector<DB::Row> erows = db->exec("SELECT DISTINCT property FROM exploit_postcondition;");

	for(auto row : qrows) {
		string prop = row[0];
		attrs.push_back(prop);
	}

    for(auto row : erows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

	return attrs;
}

vector<string> Quality::fetch_all_values() {
	vector<string> vals;
	vector<DB::Row> qrows = db->exec("SELECT DISTINCT value FROM quality;");
    vector<DB::Row> erows = db->exec("SELECT DISTINCT value FROM exploit_postcondition;");

	for(auto row : qrows) {
		string val = row[0];
		vals.push_back(val);
	}

    for(auto row : erows) {
        string val = row[0];
        vals.push_back(val);
    }

	return vals;
}

bool Quality::operator==(Quality rhs) const {
	if(this->asset_id != rhs.asset_id)
		return false;
	if(this->name != rhs.name)
		return false;
	if(this->value != rhs.value)
		return false;
	return true;
}

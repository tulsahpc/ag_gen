#include <iostream>
#include <vector>

#include "quality.h"
#include "network_state.h"
#include "util_db.h"

using namespace std;

Quality::Quality(const NetworkState &ns, int asset, string qualName, string op, string qualValue) :
        parent(ns), asset_id(asset), name(move(qualName)), op(move(op)), value(move(qualValue)) {}

string Quality::get_name() {
    return name;
}

string Quality::get_op() {
    return op;
}

void Quality::decode(size_t fact) {
    EncodedQuality eQual{};
    eQual.enc = fact;
    
    asset_id = eQual.dec.asset_id;
    name = eQual.dec.attr;
    op = eQual.dec.op;
    value = eQual.dec.val;
}

EncodedQuality Quality::encode() const {
    EncodedQuality qual {};
    qual.dec.asset_id = asset_id;
    qual.dec.attr = parent.attrs_kv[name];
    qual.dec.val = parent.vals_kv[value];

//    cout << asset_id << " " << attrs_kv[name] << " " << vals_kv[value] << " - " << qual.enc << endl;
//	cout << attrs.size() << " " << vals.size() << endl;

    return qual;
}

void Quality::print() const {
    std::cout << to_string(asset_id) + ": " + name + " => " + value << std::endl;
}

vector<Quality> Quality::fetch_all(const NetworkState &ns) {
    vector<Quality> qualities;
    vector<DB::Row> rows = db->exec("SELECT * FROM quality;");

    for (auto &row : rows) {
        int asset_id = stoi(row[0]);
        string property = row[1];
        string op = row[2];
        string value = row[3];

        Quality qual(ns, asset_id, property, op, value);
        qualities.push_back(qual);
    }

    return qualities;
}

bool Quality::operator==(const Quality &rhs) const {
    if (this->asset_id != rhs.asset_id)
        return false;
    if (this->name != rhs.name)
        return false;
    if (this->value != rhs.value)
        return false;
    return true;
}

bool Quality::operator<(const Quality &rhs) const {
    return (this->encode().enc < rhs.encode().enc);
}

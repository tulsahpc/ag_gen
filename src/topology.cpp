#include <iostream>
#include <vector>


#include "keyvalue.h"
#include "topology.h"
#include "util_db.h"

using namespace std;

Topology::Topology(int f_asset, int t_asset, string &dir,
                   string &property, string &op, string &val) :
        from_asset_id(f_asset), to_asset_id(t_asset), property(property),
        op(op), value(val), dir(dir) {}

Topology::Topology(size_t fact) {
    vector<string> attrs = fetch_all_attributes();
    vector<string> vals = fetch_all_values();

    Keyvalue<string> attrs_kv(attrs);
    Keyvalue<string> vals_kv(vals);

    EncodedTopology eTopo;
    eTopo.enc = fact;

    from_asset_id = eTopo.dec.from_asset;
    to_asset_id = eTopo.dec.to_asset;
    property = attrs_kv[eTopo.dec.property];
    op = "=";
    value = vals_kv[eTopo.dec.value];
    dir = "->";
}

int Topology::get_from_asset_id() const {
    return from_asset_id;
}

int Topology::get_to_asset_id() const {
    return to_asset_id;
}

const EncodedTopology Topology::encode() const {
    vector<string> attrs = fetch_all_attributes();
    vector<string> vals = fetch_all_values();

    Keyvalue<string> attrs_kv(attrs);
    Keyvalue<string> vals_kv(vals);

    EncodedTopology topo;

    topo.dec.from_asset = from_asset_id;
    topo.dec.to_asset = to_asset_id;
    topo.dec.dir = 0; // Assuming only one direction for now
    topo.dec.property = attrs_kv[property];
    topo.dec.op = 0; //Assuming only one operation for now
    topo.dec.value = vals_kv[value];

    return topo;
}

vector<string> Topology::fetch_all_attributes() {
    vector<string> attrs;
    vector<DB::Row> rows = db->exec("SELECT DISTINCT property FROM topology;");

    for (auto &row : rows) {
        string prop = row[0];
        attrs.push_back(prop);
    }

    return attrs;
}

vector<string> Topology::fetch_all_values() {
    vector<string> vals;
    vector<DB::Row> rows = db->exec("SELECT DISTINCT value FROM topology;");

    for (auto &row : rows) {
        string val = row[0];
        vals.push_back(val);
    }

    return vals;
}

vector<Topology> Topology::fetch_all() {
    vector<Topology> topologies;

    vector<DB::Row> rows = db->exec("SELECT * FROM topology;");
    for (auto &row : rows) {
        int from_asset = stoi(row[0]);
        int to_asset = stoi(row[1]);
        string dir = row[2];
        string property = row[3];
        string op = row[4];
        string value = row[5];

        Topology t(from_asset, to_asset, dir, property, op, value);
        topologies.push_back(t);
    }

    return topologies;
}

void Topology::print() const {
    cout << to_string(from_asset_id) + " " + dir + " " + to_string(to_asset_id) + ": " +
            property + " " + op + " " + value << endl;
}

bool Topology::operator==(const Topology &rhs) const {
    if (this->from_asset_id != rhs.from_asset_id)
        return false;
    if (this->to_asset_id != rhs.to_asset_id)
        return false;
    if (this->dir != rhs.dir)
        return false;
    if (this->property != rhs.property)
        return false;
    if (this->op != rhs.op)
        return false;
    if (this->value != rhs.value)
        return false;
    return true;
}

bool Topology::operator<(const Topology &rhs) const {
    return (this->encode().enc < rhs.encode().enc);
}

const string &Topology::getProperty() const {
    return property;
}

const string &Topology::getOp() const {
    return op;
}

const string &Topology::getValue() const {
    return value;
}

const string &Topology::getDir() const {
    return dir;
}

#include <iostream>
#include <vector>

#include "ag_gen.h"

using namespace std;

Topology::Topology(int f_asset, int t_asset, string dir, string property,
                   string op, string val)
    : from_asset_id(f_asset), to_asset_id(t_asset), property(move(property)),
      op(move(op)), value(move(val)), dir(move(dir)) {}

int Topology::get_from_asset_id() const { return from_asset_id; }
int Topology::get_to_asset_id() const { return to_asset_id; }
string Topology::get_property() const { return property; }
string Topology::get_op() const { return op; }
string Topology::get_value() const { return value; }
string Topology::get_dir() const { return dir; }

void Topology::print() const {
    cout << to_string(from_asset_id) + " " + dir + " " +
                to_string(to_asset_id) + ": " + property + " " + op + " " +
                value
         << endl;
}

const EncodedTopology Topology::encode(const Keyvalue &kv_facts) const {
    EncodedTopology topo{};

    topo.dec.from_asset = from_asset_id;
    topo.dec.to_asset = to_asset_id;
    topo.dec.dir = 0; // Assuming only one direction for now
    topo.dec.property = kv_facts[property];
    topo.dec.op = 0; // Assuming only one operation for now
    topo.dec.value = kv_facts[value];

    return topo;
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

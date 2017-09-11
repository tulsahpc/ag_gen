#include <iostream>
#include <vector>


#include "keyvalue.h"
#include "topology.h"
#include "util_db.h"
#include "util_common.h"

using namespace std;

Topology::Topology(int f_asset, int t_asset, string& opt) : from_asset_id(f_asset), to_asset_id(t_asset), options(opt) {}

Topology::Topology(size_t fact, string& opts) {
    EncodedTopology eTopo;
    eTopo.enc = fact;

    from_asset_id = eTopo.dec.from_asset;
    to_asset_id = eTopo.dec.to_asset;
    options = opts;
}

int Topology::get_from_asset_id() const {
    return from_asset_id;
}

int Topology::get_to_asset_id() const {
    return to_asset_id;
}

string Topology::get_raw_options() const {
    return options;
}

vector<string> Topology::get_options() const {
	return split(get_raw_options(), ',');
}

const EncodedTopology Topology::encode() const {
    EncodedTopology topo;
    topo.dec.from_asset = from_asset_id;
    topo.dec.to_asset = to_asset_id;

    return topo;
}

vector<Topology> Topology::fetch_all() {
    vector<Topology> topologies;

    vector<DB::Row> rows = db->exec("SELECT * FROM topology;");
    for(auto& row : rows) {
        int from_asset = stoi(row[0]);
        int to_asset = stoi(row[1]);
        string options = row[2];

		Topology t(from_asset, to_asset, options);
        topologies.push_back(t);
    }

    return topologies;
}

void Topology::print(void) const {
    cout << to_string(from_asset_id) + " => " + to_string(to_asset_id) + ": " + options << endl;
}

bool Topology::operator==(const Topology& rhs) const {
    if(this->from_asset_id != rhs.from_asset_id)
        return false;
    if(this->to_asset_id != rhs.to_asset_id)
        return false;
    if(this->options != rhs.options)
        return false;
    return true;
}

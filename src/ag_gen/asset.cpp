// asset.cpp is used to represent an asset on the network and contains functions
// to access and manipulate the properties (id, name, network, qualities) of an
// asset

#include <libpq-fe.h>
#include <string>
#include <vector>

#include "ag_gen.h"

using namespace std;

Asset::Asset(int iid, int netid, std::string nname)
    : id(iid), network_id(netid), name(move(nname)) {
    fetch_qualities();
}

// fetch_qualities grabs all of the qualities in the database associated with
// the Asset's ID and gives them to the Asset
void Asset::fetch_qualities() {
    vector<Row> rows = db->exec("SELECT * FROM quality WHERE asset_id = '" +
                                to_string(id) + "';");

    for (auto &row : rows) {
        int asset_id = stoi(row[0]);
        string property = row[1];
        string value = row[2];

        // Quality qual(asset_id, property, "=", value);
        // qualities.push_back(qual);

        qualities.emplace_back(asset_id, property, "=", value);
    }
}

// fetch_all grabs all of the Assets in the database under the network given in
// the argument and returns a vector of those Assets
vector<Asset> Asset::fetch_all(const string &network) {
    vector<Row> rows = db->exec("SELECT * FROM asset WHERE network_id = "
                                "(SELECT id FROM network WHERE name = '" +
                                network + "');");
    vector<Asset> new_assets;

    for (auto &row : rows) {
        int id = stoi(row[0]);
        string name = row[1];
        int network_id = stoi(row[2]);

        // new_assets.push_back(asset);
        new_assets.emplace_back(id, network_id, name);
    }

    return new_assets;
}
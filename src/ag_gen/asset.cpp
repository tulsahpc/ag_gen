// asset.cpp is used to represent an asset on the network and contains functions
// to access and manipulate the properties (id, name, network, qualities) of an
// asset

#include <libpq-fe.h>
#include <string>
#include <vector>

#include "ag_gen.h"

using namespace std;

/**
 * @brief Constructor for Asset
 * @details Fetches the qualities of an asset with given parameters
 *
 * @param iid The id of the asset to fetch
 * @param netid The id of the network to fetch from
 * @param nname The name of the asset
 */
Asset::Asset(int iid, int netid, std::string nname, std::vector<Quality> q)
    : id(iid), network_id(netid), name(move(nname)), qualities(q) {
    // fetch_qualities();
}

/**
 * @brief Gets all of the qualities for the Asset
 * @details Grabs all of the qualities in the database associated with
 *          the Asset's ID and gives them to the Asset
 */
/*
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
}*/

/**
 * @brief Gets all of the Assets under the network
 * @details Grabs all of the Assets in the database under the network given in
 *          the argument and returns a vector of those Assets
 *
 * @param network Name of the network to grab from
 */
/*
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
*/

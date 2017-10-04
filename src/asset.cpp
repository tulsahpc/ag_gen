// asset.cpp is used to represent an asset on the network and contains functions to access and manipulate
// the properties (id, name, network, qualities) of an asset

#include <vector>
#include <string>
#include <libpq-fe.h>


#include "asset.h"
#include "util_db.h"

using namespace std;

Asset::Asset(int id, int network_id, string asset_name) : id(id), network_id(network_id), name(asset_name) {};

// get_network_id returns the Asset's network_id
int Asset::get_network_id() {
	return network_id;
}

// get_name returns the Asset's name
string Asset::get_name() {
	return name;
}

// fetch_qualities grabs all of the qualities in the database associated with the Asset's ID and gives them
// to the Asset
void Asset::fetch_qualities()
{
	vector<DB::Row> rows = db->exec("SELECT * FROM quality WHERE asset_id = '" + to_string(id) + "';");
	vector<Quality> new_qualities;

	for(auto row : rows) {
		int asset_id = stoi(row[0]);
		string property = row[1];
		string value = row[2];

		Quality qual(asset_id, property, "=", value);
		new_qualities.push_back(qual);
	}

	this->qualities = new_qualities;
}

// fetch_all grabs all of the Assets in the database under the network given in the argument and returns a
// vector of those Assets
vector<Asset> Asset::fetch_all(string network)
{
	vector<DB::Row> rows = db->exec("SELECT * FROM asset WHERE network_id = (SELECT id FROM network WHERE name = '" + network + "');");
	vector<Asset> new_assets;

	for (auto row : rows) {
		int id = stoi(row[0]);
		string name = row[1];
		int network_id = stoi(row[2]);

		Asset asset(id, network_id, name);
		asset.fetch_qualities();

		new_assets.push_back(asset);
	}

	return new_assets;
}

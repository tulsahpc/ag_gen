//
// Created by archlord on 12/6/16.
//

#include "hashmap.h"

using namespace std;

///conn_info the string read in by read_config
///opt_network the network to hash assets from
///hash_table the hash table to fill with assets as a reference
void fill_asset_table(std::string conn_info, std::string opt_network, std::unordered_map<std::string,int>& hash_table){

    dbconnect(conn_info.c_str());
    try {
        auto network = Network::find(opt_network);
        // cout << network>id << endl;
    } catch (const exception &e) {
        cout << e.what() << endl;
    }

    vector<shared_ptr<Asset> > asset_list;
    Asset::fetch_all(asset_list, opt_network);

    for (auto asset : asset_list)
        cout << asset->name << endl;

    dbclose();

}
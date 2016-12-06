//
// Created by archlord on 12/6/16.
//

#ifndef AG_GEN_HASHMAP_H
#define AG_GEN_HASHMAP_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "network.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "util_db.h"

/*
 * At time of creation, this is not supposed to hold
 * any classes, as I will be using the std class
 * unsorted_map. This is just to help avoid conflicts in ag_gen
 */

class Keyvalue{
private:
    std::unordered_map<std::string,int> hash_table;
    std::vector<std::string>;
public:
    Keyvalue(std::string conn_info);
};


void fill_asset_table(std::string conn_info, std::string opt_network, std::unordered_map<std::string,int>& hash_table);




#endif //AG_GEN_HASHMAP_H

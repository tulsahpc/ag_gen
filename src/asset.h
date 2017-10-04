#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "quality.h"

class Asset {
	int id;
	int network_id;
	std::string name;
	std::vector<Quality> qualities;

public:
    Asset(int id, int network_id, std::string asset_name);
    int get_network_id();
    std::string get_name();

    void fetch_qualities();

    static std::vector<Asset> fetch_all(std::string network);
};

#endif //ASSET_HPP

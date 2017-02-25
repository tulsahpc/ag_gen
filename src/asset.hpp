#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "fact.hpp"

class Quality;

class Asset {
	int id;
	int network_id;
	std::string name;
	std::vector<Quality> qualities;

public:
    Asset(int id, int network_id, std::string asset_name);
    int get_network_id(void);
    std::string get_name(void);

    void fetch_qualities(void);

    static std::vector<Asset> fetch_all(const std::string& network);
};

#endif //ASSET_HPP

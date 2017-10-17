#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "quality.h"

class Asset {
	const NetworkState &parent;

	int id;
	int network_id;
	std::string name;
	std::vector<Quality> qualities;

public:
	Asset(const NetworkState &ns, int iid, int netid, std::string nname);

	std::string get_name() {
		return name;
	}

    void fetch_qualities();
    static std::vector<Asset> fetch_all(std::string network);
};

#endif //ASSET_HPP

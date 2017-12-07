#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "quality.h"

class Asset {
    int id;
    int network_id;
    std::string name;
    std::vector<Quality> qualities;

  public:
    Asset(int iid, int netid, std::string nname);

    void fetch_qualities();
    static std::vector<Asset> fetch_all(const std::string &network);
};

#endif // ASSET_HPP
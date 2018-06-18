#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "quality.h"

/** Asset class
 * @brief Fetches and saves assets
 * @details Fetches the qualities of the assets from the database and stores them in a vector.
 */
class Asset {
    int id;
    int network_id;
    std::string name;
    std::vector<Quality> qualities;

  public:
    Asset(int iid, int netid, std::string nname, std::vector<Quality> q);

    // void fetch_qualities();
    // static std::vector<Asset> fetch_all(const std::string &network);
};

#endif // ASSET_HPP

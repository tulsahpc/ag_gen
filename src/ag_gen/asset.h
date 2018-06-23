#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "quality.h"

/** Asset class
 * @brief Fetches and saves assets
 * @details Fetches the qualities of the assets from the database and stores
 * them in a vector.
 */
class Asset {
    int id;
    std::string name;
    std::vector<Quality> qualities;

  public:
    Asset(int iid, std::string nname, std::vector<Quality> q);

    std::string get_name();
};

#endif // ASSET_HPP

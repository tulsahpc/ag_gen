#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

#include "network_state.h"
#include "asset.h"
#include "util_db.h"

class Network {
    std::vector<Asset> assets;
    NetworkState initial_state;

public:
    Network(std::string &name) : assets(Asset::fetch_all(name)) {}

    int size() {
        return assets.size();
    }
};

#endif // NETWORK_H

#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <memory>

#include "network_state.h"
#include "asset.h"
#include "util_db.h"

class Network {
    std::vector<Asset> assets;
    std::unique_ptr<NetworkState> initial_state;

public:
    Keyvalue facts;

    Network(std::string &name) : assets(Asset::fetch_all(name)) {
        initial_state = generate_network_state();

        facts.populate(Quality::fetch_all_attributes());
        facts.populate(Quality::fetch_all_values());
        facts.populate(Topology::fetch_all_attributes());
        facts.populate(Topology::fetch_all_values());
    }

    NetworkState get_initial_state() {
        return *initial_state;
    }

    int size() {
        return assets.size();
    }

    std::unique_ptr<NetworkState> generate_network_state() {
        return std::unique_ptr<NetworkState>(new NetworkState(*this));
    }
};

#endif // NETWORK_H

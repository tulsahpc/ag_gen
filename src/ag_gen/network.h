#ifndef NETWORK_H
#define NETWORK_H

#include <memory>
#include <vector>

#include "asset.h"
#include "network_state.h"
#include "util/db.h"
#include "util/keyvalue.h"

/** Network class
 * @brief Holds information about the Network
 * @details Fetches assets a populates facts with a given name
 *          and generates an initial NetworkState.
 */
class Network {
    std::vector<Asset> assets;
    std::unique_ptr<NetworkState> initial_state;

  public:
    Keyvalue facts;

    /**
     * @brief Constructor for Network
     * @details Fetches assets, generates an initial NetworkState,
     *          and populates facts.
     *
     * @param name Name of the network
     */
    Network(std::string &name, std::vector<Asset> a, Keyvalue f) : assets(a), facts(f) {
        initial_state = generate_network_state();

        // facts.populate(Quality::fetch_all_attributes());
        // facts.populate(Quality::fetch_all_values());
        // facts.populate(Topology::fetch_all_attributes());
        // facts.populate(Topology::fetch_all_values());
    }

    /**
     * @brief Returns the initial NetworkState
     */
    NetworkState get_initial_state() { return *initial_state; }

    /**
     * @brief Returns the size of the Assets vector
     */
    int size() { return assets.size(); }

    /**
     * @brief Generates a NetworkState
     */
    std::unique_ptr<NetworkState> generate_network_state() {
        return std::make_unique<NetworkState>(*this);
    }
};

#endif // NETWORK_H

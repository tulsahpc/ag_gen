#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>
#include <deque>
#include <map>
#include <set>

#include "network_state.h"

#include "edge.h"

class AGGen {
    Keyvalue<Asset> assets; // Simple hashtable that stores the asset id with its name. Holds all assets in a network.
//    Keyvalue<std::string> attrs; // All possible attributes used in the network.
//    Keyvalue<std::string> vals; // All possible values used in the network.

    std::deque<NetworkState> frontier; // The newly generated states to go through next
    std::map<size_t, Factbase> fb_list {};
    std::set<Edge> edge_list{};

    std::vector<std::tuple<Exploit, AssetGroup> > check_exploits(const NetworkState &s);

    bool check_assetgroup(const NetworkState &s, const AssetGroup &assetgroup);

    std::vector<AssetGroup> gen_hypo_facts(const NetworkState &s, Exploit &e);

    std::tuple<std::vector<Quality>, std::vector<Topology> >
    createPostConditions(const NetworkState &s, std::tuple<Exploit, AssetGroup> group);

public:
    explicit AGGen(NetworkState initial_state);

    void generate();
};

#endif //AG_GEN_HPP

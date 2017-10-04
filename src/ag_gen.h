#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>

#include "asset.h"
#include "keyvalue.h"
#include "exploit.h"
#include "factbase.h"
#include "assetgroup.h"
#include "network_state.h"

class AGGen {
    Keyvalue<Asset> assets; // Simple hashtable that stores the asset id with its name. Holds all assets in a network.
    Keyvalue<std::string> attrs; // All possible attributes used in the network.
    Keyvalue<std::string> vals; // All possible values used in the network.

    std::vector<NetworkState> frontier; // The newly generated states to go through next
    std::vector<size_t> hash_list;

    std::vector<std::tuple<Exploit, AssetGroup> > check_exploits(NetworkState&);
    bool check_assetgroup(NetworkState&, AssetGroup&);
    std::vector<AssetGroup> gen_hypo_facts(NetworkState&, Exploit&);
    std::tuple<std::vector<Quality>, std::vector<Topology> > createPostConditions(std::tuple<Exploit, AssetGroup> group);
public:
	AGGen(NetworkState&);
    void generate();
};

#endif //AG_GEN_HPP

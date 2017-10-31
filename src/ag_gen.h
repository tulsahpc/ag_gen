#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>
#include <deque>
#include <map>
#include <set>

#include "asset.h"
#include "keyvalue.h"
#include "exploit.h"
#include "factbase.h"
#include "assetgroup.h"
#include "network_state.h"
#include "edge.h"

class AGGen {
    std::deque<NetworkState> frontier; // The newly generated states to go through next
    std::map<size_t, NetworkState> state_list {};
    std::set<Edge> edge_list {};

    std::vector<std::tuple<Exploit, AssetGroup> > check_exploits(const NetworkState &s);
    bool check_assetgroup(const NetworkState &s, const AssetGroup &assetgroup);

    std::vector<AssetGroup> gen_hypo_facts(const NetworkState &s, Exploit &e);
    std::tuple<std::vector<Quality>, std::vector<Topology> > createPostConditions(std::tuple<Exploit, AssetGroup> group);

public:
    explicit AGGen(NetworkState initial_state);

    void generate();
};

#endif //AG_GEN_HPP

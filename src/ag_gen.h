#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <list>
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
    std::set<size_t> state_list {};
    std::set<Edge> edge_list {};

    std::tuple<std::vector<Quality>, std::vector<Topology> > createPostConditions(std::tuple<Exploit, AssetGroup> group);

public:
    explicit AGGen(NetworkState initial_state);

    void generate();
};

#endif //AG_GEN_HPP

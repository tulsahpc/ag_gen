#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <list>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <tuple>

#include "asset.h"
#include "keyvalue.h"
#include "exploit.h"
#include "factbase.h"
#include "assetgroup.h"
#include "network_state.h"
#include "edge.h"
#include "network.h"

class AGGen {
    Network &net;

    std::deque<NetworkState> frontier;
    
    std::set<NetworkState> state_list {};
    std::set<size_t> hash_list {};
    std::set<Edge> edge_list {};

    std::tuple<std::vector<Quality>, std::vector<Topology> > createPostConditions(std::tuple<Exploit, AssetGroup> &group);

public:
    explicit AGGen(Network &net_i, DB &db);

    void generate();

    static std::shared_ptr<DB> db;
};

#endif //AG_GEN_HPP

#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <deque>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "asset.h"
#include "assetgroup.h"
#include "edge.h"
#include "exploit.h"
#include "factbase.h"
#include "network.h"
#include "network_state.h"

#include "util/keyvalue.h"

class AGGen {
    Network &net;

    std::deque<NetworkState> frontier;

    std::set<size_t> hash_list{};

    std::tuple<std::vector<Quality>, std::vector<Topology>>
    createPostConditions(std::tuple<Exploit, AssetGroup> &group);

  public:
    explicit AGGen(Network &net_i);
    void generate();
};

#endif // AG_GEN_HPP
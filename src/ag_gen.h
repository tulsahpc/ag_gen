#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>

#include "asset.h"
#include "keyvalue.h"
#include "exploit.h"
#include "factbase.h"
#include "assetgroup.h"
#include "network_state.h"

class AttackNode {
    const Factbase& fb;
    std::vector<AttackNode> children;
public:
    AttackNode(const Factbase& ifb): fb(ifb) {};
};

class AGGen {
    Keyvalue<Asset> assets;
    Keyvalue<std::string> attrs;
    Keyvalue<std::string> vals;

    std::vector<NetworkState> frontier;
    std::vector<size_t> hash_list;

    std::vector<std::tuple<Exploit, AssetGroup> > check_exploits(NetworkState&);
    bool check_assetgroup(NetworkState&, AssetGroup&);
    std::vector<AssetGroup> gen_hypo_facts(NetworkState&, Exploit&);
    std::tuple<std::vector<Quality>, std::vector<Topology> > createPostConditions(std::tuple<Exploit, AssetGroup> group);
public:
	AGGen(const NetworkState&);
    void generate();
};

#endif //AG_GEN_HPP

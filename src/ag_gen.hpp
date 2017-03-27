#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>
#include "asset.hpp"
#include "keyvalue.hpp"
#include "exploit.hpp"
#include "factbase.hpp"

class AGGen {
    Keyvalue<Asset> assets;
    Keyvalue<std::string> attrs;
    Keyvalue<std::string> vals;

    Factbase facts;

    std::vector<std::tuple<Exploit, AssetGroup> > check_exploits(void);
    bool check_assetgroup(AssetGroup assetgroup);
    std::vector<AssetGroup> gen_hypo_facts(Exploit& e);
public:
	AGGen(void);
};

#endif //AG_GEN_HPP

#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <vector>

#include "asset.h"
#include "keyvalue.h"
#include "exploit.h"
#include "factbase.h"
#include "assetgroup.h"

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

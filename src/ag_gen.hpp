#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include "keyvalue.hpp"

class AGGen {
    void gen_hypo_facts(void);
public:
	Keyvalue<Asset> assets;
	Keyvalue<std::string> attrs;
	Keyvalue<std::string> vals;

	AGGen(void);
};

#endif //AG_GEN_HPP

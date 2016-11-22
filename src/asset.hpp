#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <vector>
#include <memory>
#include "fact.hpp"

class Fact;
class Quality;

class Asset {
public:
	int network_id;
    std::string name;
    std::vector<std::reference_wrapper<Fact> > facts;

    Asset(int network_id, std::string const& asset_name);
    std::string getName();

    static int fetch_all(std::vector<std::shared_ptr<Asset> > &asset_list, std::string const &network);
    static int fetch_qualities(std::vector<std::shared_ptr<Quality> > &quality_list, int asset_id);
};

#endif //ASSET_HPP

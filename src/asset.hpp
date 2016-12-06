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

    std::vector<std::shared_ptr<Asset>> Asset::fetch_all(std::string const &network);
    static int fetch_qualities(std::vector<std::shared_ptr<Quality> > &quality_list, int asset_id);
};

#endif //ASSET_HPP

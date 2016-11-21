#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "asset.hpp"

class Network {
public:
	int id;
	std::string name;
	std::vector<Asset> assets;

	static std::shared_ptr<Network> find_network(const std::string net);
	static int networks_fetch(std::vector<std::shared_ptr<Network> > &network_list);
};

#endif //NETWORK_APP

/** \file ag_network.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <vector>

struct Network {
	int id;
	std::string name;
};

int networks_fetch(std::vector<std::shared_ptr<Network> > &);
std::shared_ptr<Network> find_network(const std::string);

#endif //C_AG_NETWORK_H

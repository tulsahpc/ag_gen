/** \file ag_network.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
using namespace std;

struct Network {
	int id;
	string name;
};

int networks_fetch(vector<Network *> &);

#endif //C_AG_NETWORK_H

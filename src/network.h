/** \file ag_network.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "util_list.h"

struct Network {
	int id;
	char *name;
};

int networks_fetch(struct List *);
void network_free(struct Network *);

#endif //C_AG_NETWORK_H

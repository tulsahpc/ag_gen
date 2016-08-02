/** \file ag_network.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef NETWORK_H
#define NETWORK_H

struct Network {
	int id;
	char *name;
};

struct List *networks_fetch(void);
int network_free(struct Network *);

#endif //C_AG_NETWORK_H

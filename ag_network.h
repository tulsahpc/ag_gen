#ifndef C_AG_NETWORK_H
#define C_AG_NETWORK_H

/**
 * Struct that contains a pointer to a list of networks and the length of that list.
 *
 *
 */
struct AGNetworkList {
	struct AGNetwork **networks;
	int len;
};

/**
 * Struct that defines a network as having an id and a name.
 *
 *
 */
struct AGNetwork {
	int id;
	char *name;
};

struct AGNetworkList *AGGetNetworks(void);
int AGNetworksFree(struct AGNetworkList *);
void AGNetworksPrint(const struct AGNetworkList *);

#endif //C_AG_NETWORK_H

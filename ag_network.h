#ifndef C_AG_NETWORK_H
#define C_AG_NETWORK_H

struct AGNetworkList {
	struct AGNetwork **networks;
	int len;
};

struct AGNetwork {
	int id;
	char *name;
};

struct AGNetworkList *AGGetNetworks(void);
int AGNetworkFree(struct AGNetwork *);
int AGNetworkListFree(struct AGNetworkList *);
int AGNetworkListPrint(const struct AGNetworkList *);

#endif //C_AG_NETWORK_H

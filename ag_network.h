//
// Created by Kyle Cook on 5/26/16.
//

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
int AGFreeNetworks(struct AGNetworkList *networkList);
void printNetworks(const struct AGNetworkList*);

#endif //C_AG_NETWORK_H

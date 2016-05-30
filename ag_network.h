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
int AGNetworksFree(struct AGNetworkList *);
void AGNetworksPrint(const struct AGNetworkList *);

int AGNetworkNew(const char *);
int AGNetworkDestroy(int);
int AGNetworkSave(int);
int AGNetworkCreateAsset(const char *);
int AGNetworkRemoveAsset(int);
int AGNetworkFindAsset(const struct AGNetwork *, const char *);
int AGNetworkAllAssets(const struct AGNetwork *);

#endif //C_AG_NETWORK_H

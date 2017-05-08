#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "factbase.h"

class NetworkState {
    Factbase factbase;
public:
    NetworkState(void);

    Factbase& get_factbase(void);
};

#endif

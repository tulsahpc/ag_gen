//
// Created by Kyle Cook on 5/27/16.
//

#ifndef C_AG_FACTS_H
#define C_AG_FACTS_H

enum PROPERTY { OS, Thing };

struct AGQuality {
    int asset_id;
    char *property;
    char *value;
};

struct AGTopology {

};

union AGFact {
    struct AGQuality*;
    struct AGTopology*;
};

#endif //C_AG_FACTS_H

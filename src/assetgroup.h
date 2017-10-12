//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_ASSET_GROUP_H
#define AG_GEN_ASSET_GROUP_H

#include <vector>

#include "quality.h"
#include "topology.h"

struct AssetGroup {
    std::vector<Quality> hypothetical_qualities;
    std::vector<Topology> hypothetical_topologies;
    std::vector<int> perm;

    void print_facts(void);

    void print_group(void);
};


#endif //AG_GEN_ASSET_GROUP_H

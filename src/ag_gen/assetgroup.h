//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_ASSET_GROUP_H
#define AG_GEN_ASSET_GROUP_H

#include <vector>

#include "quality.h"
#include "topology.h"

class AssetGroup {
    std::vector<Quality> hypothetical_qualities;
    std::vector<Topology> hypothetical_topologies;

    std::vector<int> perm;

  public:
    AssetGroup(std::vector<Quality> hypo_quals,
               std::vector<Topology> hypo_topos, std::vector<int> pperm)
        : hypothetical_qualities(move(hypo_quals)),
          hypothetical_topologies(move(hypo_topos)), perm(move(pperm)) {}

    std::vector<int> get_perm() const { return perm; }

    std::vector<Quality> get_hypo_quals() const {
        return hypothetical_qualities;
    }

    std::vector<Topology> get_hypo_topos() const {
        return hypothetical_topologies;
    }

    void print_facts();
    void print_group();
};

#endif // AG_GEN_ASSET_GROUP_H

#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>
#include <functional>
#include <cstdint>

#include "quality.h"
#include "topology.h"

struct FactbaseHash;

class Factbase {
    int id;
    std::vector<Quality> qualities;
    std::vector<Topology> topologies;

public:
    void populate();

    bool save();

    bool find_quality(Quality &) const;

    bool find_topology(Topology &) const;

    void add_quality(Quality);

    void add_topology(Topology);

    bool exists_in_db();

    void print() const;

    int get_id() const;

    size_t hash() const;
};

#endif

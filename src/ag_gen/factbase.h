#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <cstdint>
#include <functional>
#include <vector>

#include "quality.h"
#include "topology.h"

class NetworkState;
class Factbase {
    const NetworkState *parent;

    int id;
    std::vector<Quality> qualities;
    std::vector<Topology> topologies;

    Factbase();
    void set_parent(const NetworkState &ns);

    friend class NetworkState;

  public:
    void populate();
    void save();

    bool find_quality(Quality &q) const;
    bool find_topology(Topology &t) const;

    void add_quality(Quality &q);
    void add_topology(Topology &y);

    bool exists_in_db();

    void print() const;
    int get_id() const;
    size_t hash() const;
};

#endif

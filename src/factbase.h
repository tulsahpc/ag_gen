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
    Factbase();

    Factbase(const Factbase &fb);

    Factbase(Factbase &&fb);

    explicit Factbase(int id);

    Factbase &operator=(const Factbase &fb);

    Factbase &operator=(Factbase &&fb);

    void populate();

    bool find_quality(Quality &) const;

    void add_quality(Quality);

    bool find_topology(Topology &) const;

    void add_topology(Topology);

    bool exists_in_db();

    int get_id() const;

    void save();

    size_t hash() const;

    void print();
};

#endif

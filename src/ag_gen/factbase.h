#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <cstdint>
#include <functional>
#include <vector>

#include "quality.h"
#include "topology.h"

class NetworkState;

/** Factbase class
 * @brief Contains known facts in a NetworkState.
 * @details Contains known facts that are completely true in the
 *          NetworkState such as Qualities and Topologies.
 */
class Factbase {
    static int current_id;
    const NetworkState *parent;

    int id;
    std::vector<Quality> qualities;
    std::vector<Topology> topologies;

    Factbase(std::vector<Quality> q, std::vector<Topology> t);
    void set_parent(const NetworkState &ns);

    friend class NetworkState;

  public:

    bool find_quality(Quality &q) const;
    bool find_topology(Topology &t) const;

    void add_quality(Quality &q);
    void add_topology(Topology &y);

    void print() const;
    void set_id();
    int get_id() const;
    size_t hash(Keyvalue &factlist) const;
};

#endif

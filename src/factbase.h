#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>

#include "network_state.h"
#include "quality.h"
#include "topology.h"

class Quality;
class NetworkState;

class Factbase {
    int id;
    std::vector<Quality> qualities;
    std::vector<Topology> topologies;

public:
    void populate(const NetworkState &ns);
    void save();

    bool find_quality(Quality &) const;
    bool find_topology(Topology &) const;

    void add_quality(Quality q);
    void add_topology(Topology t);
	bool exists_in_db();
	
	void print() const;
	int get_id() const;

    size_t hash() const;
};

#endif

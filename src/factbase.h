#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>

#include "quality.h"
#include "topology.h"

class Factbase {
	std::vector<Quality> qualities;
	std::vector<Topology> topologies;
public:
	Factbase(void);
	bool find_quality(Quality);
	bool find_topology(Topology);
	void add_quality(Quality);
	void add_topology(Topology);
};

#endif

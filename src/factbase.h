#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>

#include "quality.h"
#include "topology.h"

class Factbase {
	std::vector<const Quality> qualities;
	std::vector<const Topology> topologies;
public:
	Factbase(void);
	Factbase(const Factbase&);

	bool find_quality(const Quality&) const;
	void add_quality(const Quality&);

	bool find_topology(const Topology&) const;
	void add_topology(const Topology&);

    void print(void) const;
};

#endif

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
	Factbase(const Factbase&);

	bool find_quality(const Quality&) const;
//	bool find_quality(Quality&);
	void add_quality(const Quality&) const;
//	void add_quality(Quality&);

	bool find_topology(const Topology&) const;
//	bool find_topology(Topology&);
	void add_topology(const Topology&) const;
//	void add_topology(Topology&);

    void print(void);
};

#endif

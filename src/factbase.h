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
	Factbase(Factbase&);

	bool find_quality(Quality&);
	void add_quality(Quality&);

	bool find_topology(Topology&);
	void add_topology(Topology&);

    void print(void);
>>>>>>> 9a036bddf759a46cd5fcb48810208f6a8a6af5d7
};

#endif

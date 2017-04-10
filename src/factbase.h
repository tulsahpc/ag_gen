#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>

#include "fact.h"

class Factbase {
	std::vector<Quality> qualities;
	std::vector<Topology> topologies;
public:
	Factbase(void) {
		qualities = Quality::fetch_all();
		topologies = Topology::fetch_all();
	}

	bool find_quality(Quality q) {
		if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
			return false;
		}
		return true;
	}

	bool find_topology(Topology t) {
		if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
			return false;
		}
		return true;
	}

	void add_quality(Quality q) {
		qualities.push_back(q);
	}

	void add_topology(Topology t) {
		topologies.push_back(t);
	}
};

#endif

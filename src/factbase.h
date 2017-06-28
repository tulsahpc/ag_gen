#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>
#include <functional>
#include <cstdint>

#include "quality.h"
#include "topology.h"

class Factbase;
struct FactbaseHash;

class Factbase {
	std::vector<Quality> qualities;
	std::vector<Topology> topologies;

	int id;
    int hash_value;

    Factbase get(const int id) const;
	int request_id(void);
public:
	Factbase(void);
	Factbase(const Factbase&);

	bool find_quality(Quality&);
	void add_quality(Quality&);

	bool find_topology(Topology&);
	void add_topology(Topology&);

	void save(void);
    void print(void) const;

	size_t hash(void);

    friend struct FactbaseHash;
};

struct FactbaseHash {
    size_t combine(size_t seed) const {
        seed ^= std::hash<size_t>{}(seed) +
                0x9e3779b97f4a7c15 +
                (seed << 6) +
                (seed >> 2);
        return seed;
    }

    size_t operator()(const Factbase& fb) const {
//        size_t hash = 0xf848b64e; // Random seed value
        size_t hash = 0x0c32a12fe19d2119;
        for(auto& qual : fb.qualities) {
            const EncodedQuality encoded = qual.encode();
            hash ^= combine(encoded.enc);
        }
        for(auto& topo : fb.topologies) {
            const EncodedTopology encoded = topo.encode();
            hash ^= combine(encoded.enc);
        }
        return hash;
    }
};

#endif

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

	int new_id(void);
public:
	Factbase(void);
	Factbase(const Factbase&);
	Factbase(int id);

	void populate(void);

	bool find_quality(Quality&) const;
	void add_quality(Quality&);

	bool find_topology(Topology&) const;
	void add_topology(Topology&);

	bool exists_in_db(void);
	int get_id(void);
	void save(void);

	size_t hash(void);

    friend struct FactbaseHash;
};

struct FactbaseHash {
	// Shamelessly copied from Boost::hash_combine
    size_t combine(size_t seed) {
        seed ^= std::hash<size_t>{}(seed) +
                0x9e3779b97f4a7c15 +
                (seed << 6) +
                (seed >> 2);
        return seed;
    }

    size_t operator()(Factbase& fb) {
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

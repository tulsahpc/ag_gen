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

    static size_t hash(const Factbase&);

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
            EncodedQuality encoded = qual.encode();
            hash ^= combine(encoded.enc);
        }
        return hash;
    }
};

#endif

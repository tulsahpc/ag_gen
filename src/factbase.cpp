// factbase.cpp contains a default constructor, standard setter and getter methods for the qualities and
// topologies of factbase, and a print method that prints the qualities and topologies of a factbase

#include <iostream>
#include <algorithm>
#include "factbase.h"

// The Factbase constructor creates a factbase object
Factbase::Factbase(void) {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

Factbase::Factbase(Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {}

// find_quality searches for a given quality in a factbase. Returns true if the quality is found, otherwise
// returns false
bool Factbase::find_quality(Quality& q) {
    if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

// find_topology searches for a given topology in a factbase. Returns true if the topology is found, otherwise
// returns false
bool Factbase::find_topology(Topology& t) {
    if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

// add_quality adds a given quality to the factbase's vector of qualities
void Factbase::add_quality(Quality& q) {
    qualities.push_back(q);
}

// add_topology adds a given topology to the factbase's vector of topologies
void Factbase::add_topology(Topology& t) {
    topologies.push_back(t);
}

// print prints to stdout every quality of the factbase then every topology of the factbase
void Factbase::print(void) {
    for(auto& q : qualities) {
        q.print();
    }

    for(auto& t : topologies) {
        t.print();
    }
}

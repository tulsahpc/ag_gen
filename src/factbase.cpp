#include <iostream>
#include <algorithm>
#include "factbase.h"

Factbase::Factbase(void) {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

Factbase::Factbase(Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {}

bool Factbase::find_quality(Quality& q) {
    if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

bool Factbase::find_topology(Topology& t) {
    if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

void Factbase::add_quality(Quality& q) {
    qualities.push_back(q);
}

void Factbase::add_topology(Topology& t) {
    topologies.push_back(t);
}

void Factbase::print(void) {
    for(auto& q : qualities) {
        q.print();
    }

    for(auto& t : topologies) {
        t.print();
    }
}

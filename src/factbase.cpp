#include <iostream>
#include <algorithm>
#include "factbase.h"

Factbase::Factbase(void) {
    qualities = Quality::fetch_all();
    topologies = Topology::fetch_all();
}

Factbase::Factbase(const Factbase& fb) : qualities(fb.qualities), topologies(fb.topologies) {}

bool Factbase::find_quality(const Quality& q) const {
    if(find(qualities.begin(), qualities.end(), q) == qualities.end()) {
        return false;
    }
    return true;
}

//bool Factbase::find_quality(Quality& q) {
//    return
//}

void Factbase::add_quality(const Quality& q) const {
    qualities.push_back(q);
}

//void Factbase::add_quality(Quality& q) {
//
//}

bool Factbase::find_topology(const Topology& t) const {
    if(find(topologies.begin(), topologies.end(), t) == topologies.end()) {
        return false;
    }
    return true;
}

//bool Factbase::find_topology(Topology& t) {
//
//}

void Factbase::add_topology(const Topology& t) const {
    topologies.push_back(t);
}

//void Factbase::add_topology(Topology& t) {
//
//}

void Factbase::print(void) {
    for(auto& q : qualities) {
        q.print();
    }

    for(auto& t : topologies) {
        t.print();
    }
}

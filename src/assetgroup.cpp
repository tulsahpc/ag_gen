//
// Created by Kyle Cook on 4/9/17.
//
// assetgroup.cpp is used to hold the hypothetical qualities and topologies of multiple assets as well as a
// vector of the number IDs of the assets. It also implements a print method for the qualities and topologies
// and for the assets.

#include <iostream>

#include "assetgroup.h"

using namespace std;

// print_facts prints all of the hypothetical qualities of an Asset Group, then prints all of the
// hypothetical topologies
void AssetGroup::print_facts(void) {
    for (auto &quality : this->hypothetical_qualities) {
        quality.print();
    }

    for (auto &topology : this->hypothetical_topologies) {
        topology.print();
    }
    cout << endl;
}

// print_group prints each asset to stdout on a single line in a comma seperated list
void AssetGroup::print_group(void) {
    if(perm.size() == 1) {
        cout << "Asset " + to_string(perm[0]);
    } else {
        cout << "Assets " + to_string(perm[0]);
        for (auto i = 0; i < perm.size(); i++) {
            cout << ", " + to_string(perm[i]);
        }
    }
    cout << endl;
}

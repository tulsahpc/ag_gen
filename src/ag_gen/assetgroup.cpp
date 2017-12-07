//
// Created by Kyle Cook on 4/9/17.
//
// assetgroup.cpp is used to hold the hypothetical qualities and topologies of
// multiple assets as well as a vector of the number IDs of the assets. It also
// implements a print method for the qualities and topologies and for the
// assets.

#include <iostream>

#include "assetgroup.h"

using namespace std;

// print_facts prints all of the hypothetical qualities of an Asset Group, then
// prints all of the hypothetical topologies
void AssetGroup::print_facts() {
    for (auto &quality : this->get_hypo_quals()) {
        quality.print();
    }

    for (auto &topology : this->get_hypo_topos()) {
        topology.print();
    }
    cout << endl;
}

// print_group prints each asset to stdout on a single line in a comma seperated
// list
void AssetGroup::print_group() {
    if (perm.size() == 1) {
        cout << "Asset " + to_string(perm[0]);
    } else {
        cout << "Assets " + to_string(perm[0]);
        for (int i : perm) {
            cout << ", " + to_string(i);
        }
    }
    cout << endl;
}

//
// Created by Kyle Cook on 4/9/17.
//

#include <iostream>

#include "assetgroup.h"

using namespace std;

void AssetGroup::print_facts(void) {
    for (auto &quality : this->hypothetical_qualities) {
        cout << "\t\t";
        quality.print();
    }

    for (auto &topology : this->hypothetical_topologies) {
        cout << "\t\t";
        topology.print();
    }
    cout << endl;
}

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

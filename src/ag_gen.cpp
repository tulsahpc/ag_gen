#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <getopt.h>

#include "ag_gen.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "util_common.hpp"
#include "util_db.hpp"
#include "util_odometer.hpp"
#include "keyvalue.hpp"
#include "config.hpp"
#include "global.hpp"

using namespace std;

AGGen::AGGen(void) : assets(Asset::fetch_all("home")), attrs(Quality::fetch_all_attributes()), vals(Quality::fetch_all_values()) {

    gen_hypo_facts();
}

void AGGen::gen_hypo_facts(void) {
    int qual_len = attrs.length();
    int val_len = vals.length();

    vector<Exploit> exploit_list = Exploit::fetch_all();
    for_each(exploit_list.begin(), exploit_list.end(), [](Exploit& e) {
        cout << "Exploit: " + e.get_name() << endl;
        e.print_preconds();
    });

    Odometer<3,2> od;
    // od.print();
}

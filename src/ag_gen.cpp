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

bool check_assetgroup(vector<Quality> assetgroup) {
    auto real_factbase = Quality::fetch_all();
    for(auto quality : assetgroup) {
        if(find(real_factbase.begin(), real_factbase.end(), quality) == real_factbase.end())
            return false;
    }
    return true;
}

void AGGen::gen_hypo_facts(void) {
    vector<Exploit> exploit_list = Exploit::fetch_all();
    // for(auto i=0; i<exploit_list.size(); i++) {
    for(auto e : exploit_list) {
        int num_assets = assets.length();
        int num_params = e.get_num_params();
        
        vector<ParameterizedQuality> preconds = e.precond_list();
        Odometer od(num_params, num_assets);
        vector<vector<Quality> > hypothetical_facts;

        for(auto j=0; j<od.length(); j++) {
            vector<int> perm = od.next();
            vector<Quality> asset_group;
            for(auto precond : preconds) {
                Quality q(perm[precond.get_param_num()], precond.get_name(), precond.get_value());
                asset_group.push_back(q);
            }
            hypothetical_facts.push_back(asset_group);
        }
        od.reset();

        cout << "Exploit " + e.get_name() + ": " << endl;

        auto count = 0;
        for(auto asset_group : hypothetical_facts) {
            // Each quality must exist. If not, discard asset_group entirely.
            bool applicable = check_assetgroup(asset_group);
            if(applicable) {
                cout << "\tGroup " + to_string(count) << endl;
                for(auto quality : asset_group) {
                    cout << "\t\t";
                    quality.print();
                }
                cout << endl;
            }
            count++;
        }
        cout << endl;

        // int counter = 0;
        // for(auto asset_group : hypothetical_facts) {
        //     cout << "Asset Group: " + to_string(counter++) << endl;
        // }
    }
}

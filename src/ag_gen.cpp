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

class AssetGroup {
public:
    vector<Quality> group;
};

AGGen::AGGen(void) : assets(Asset::fetch_all("home")), attrs(Quality::fetch_all_attributes()), vals(Quality::fetch_all_values()) {

    gen_hypo_facts();
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

        int counter = 0;
        for(auto asset_group : hypothetical_facts) {
            cout << "Asset Group: " + to_string(counter++) << endl;
            for(auto quality : asset_group) {
                quality.print();
            }
            cout << endl;
        }
    }
}

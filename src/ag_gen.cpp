#include <iostream>
#include <unordered_map>

#include "ag_gen.hpp"
#include "exploit.hpp"
#include "util_odometer.hpp"
#include "fact.hpp"

using namespace std;

AGGen::AGGen(void) : assets(Asset::fetch_all("home")), attrs(Quality::fetch_all_attributes()), vals(Quality::fetch_all_values()) {

    gen_hypo_facts();
}

bool check_assetgroup(AssetGroup assetgroup) {
    auto real_factbase_q = Quality::fetch_all();
    auto real_factbase_t = Topology::fetch_all();

    for(auto& quality : assetgroup.hypothetical_qualities) {
        if(find(real_factbase_q.begin(), real_factbase_q.end(), quality) == real_factbase_q.end()) {
            return false;
        }
    }

    for(auto& topology : assetgroup.hypothetical_topologies) {
        if(find(real_factbase_t.begin(), real_factbase_t.end(), topology) == real_factbase_t.end()) {
            return false;
        }
    }

    return true;
}

void AGGen::gen_hypo_facts(void) {
    vector<Exploit> exploit_list = Exploit::fetch_all();
    // for(auto i=0; i<exploit_list.size(); i++) {
    for(auto& e : exploit_list) {
        int num_assets = assets.length();
        int num_params = e.get_num_params();
        
        vector<ParameterizedQuality> preconds_q = e.precond_list_q();
        vector<ParameterizedTopology> preconds_t = e.precond_list_t();
        Odometer od(num_params, num_assets);
        vector<AssetGroup> asset_groups;

        for(auto j=0; j<od.length(); j++) {
            vector<int> perm = od.next();

            vector<Quality> asset_group_quals;
            vector<Topology> asset_group_topos;

            for(auto& precond : preconds_q) {
                Quality q(perm[precond.get_param_num()], precond.get_name(), precond.get_value());
                asset_group_quals.push_back(q);
            }

            for(auto& precond : preconds_t) {
                Topology t(perm[precond.get_from_param()], perm[precond.get_to_param()], precond.get_options());
                asset_group_topos.push_back(t);
            }

            AssetGroup exploit_asset_group = { asset_group_quals, asset_group_topos };
            asset_groups.push_back(exploit_asset_group);
        }
        od.reset();

        cout << "Exploit " + e.get_name() + ": " << endl;

        auto count = 0;
        for(auto asset_group : asset_groups) {
            // Each quality must exist. If not, discard asset_group entirely.
            bool applicable = check_assetgroup(asset_group);
            if(applicable) {
                cout << "\tGroup " + to_string(count) << endl;
                for(auto& quality : asset_group.hypothetical_qualities) {
                    cout << "\t\t";
                    quality.print();
                }

                for(auto& topology : asset_group.hypothetical_topologies) {
                    cout << "\t\t";
                    topology.print();
                }
                cout << endl;
            }
            count++;
        }
        cout << endl;
    }
}

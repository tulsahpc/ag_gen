#include <iostream>
#include <vector>

#include "ag_gen.h"
#include "util_odometer.h"

using namespace std;

AGGen::AGGen(void) : assets(Asset::fetch_all("home")), attrs(Quality::fetch_all_attributes()), vals(Quality::fetch_all_values()) {
    vector<tuple<Exploit, AssetGroup> > appl_exploits = check_exploits();

    for_each(appl_exploits.begin(), appl_exploits.end(), [](tuple<Exploit, AssetGroup> e) {
        Exploit ex = get<0>(e);
        AssetGroup ag = get<1>(e);

        ex.print_id();
        ag.print_group();
        ex.print_postconds_q();
        ex.print_postconds_t();
    });
}

bool AGGen::check_assetgroup(AssetGroup assetgroup) {
    for(auto& quality : assetgroup.hypothetical_qualities) {
        if(!facts.find_quality(quality)) {
            return false;
        }
    }

    for(auto& topology : assetgroup.hypothetical_topologies) {
        if(!facts.find_topology(topology)) {
            return false;
        }
    }

    return true;
}

vector<AssetGroup> AGGen::gen_hypo_facts(Exploit& e) {
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

        AssetGroup exploit_asset_group = { asset_group_quals, asset_group_topos, perm };
        asset_groups.push_back(exploit_asset_group);
    }
    od.reset();

    return asset_groups;
}

vector<tuple<Exploit, AssetGroup> > AGGen::check_exploits(void) {
    vector<Exploit> exploit_list = Exploit::fetch_all();
    vector<tuple<Exploit, AssetGroup> > appl_exploit_list;

    for(auto& e : exploit_list) {
        vector<AssetGroup> asset_groups = gen_hypo_facts(e);
        for(auto asset_group : asset_groups) {
            // Each quality must exist. If not, discard asset_group entirely.
            bool applicable = check_assetgroup(asset_group);
            if(applicable) {
                appl_exploit_list.push_back(make_tuple(e, asset_group));
            }
        }
    }

    return appl_exploit_list;
}

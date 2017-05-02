#include <iostream>
#include <vector>

#include "ag_gen.h"
#include "network_state.h"

#include "util_odometer.h"

using namespace std;

AGGen::AGGen(NetworkState initial_state) : assets(Asset::fetch_all("home")), attrs(Quality::fetch_all_attributes()), vals(Quality::fetch_all_values()), current_state(initial_state) {
    auto appl_exploits = check_exploits();

    vector<reference_wrapper<NetworkState> > new_states;
    int count = 0;
    // All of these exploits are applicable
    for(auto& e : appl_exploits) {
        auto postconditions = createPostConditions(e);
        auto qualities = get<0>(postconditions);
        auto topologies = get<1>(postconditions);

        NetworkState new_state(current_state);

        for(auto& qual : qualities) {
            new_state.get_factbase().add_quality(qual);
        }

        for(auto& topo : topologies) {
            new_state.get_factbase().add_topology(topo);
        }

        new_states.push_back(ref(new_state));
    }

    for(auto& state : new_states) {
        state.get().print();
    }
}

bool AGGen::check_assetgroup(AssetGroup& assetgroup) {
    auto& current_facts = current_state.get_factbase();

    for(auto& quality : assetgroup.hypothetical_qualities) {
        if(!current_facts.find_quality(quality)) {
            return false;
        }
    }

    for(auto& topology : assetgroup.hypothetical_topologies) {
        if(!current_facts.find_topology(topology)) {
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

tuple<vector<Quality>, vector<Topology> > AGGen::createPostConditions(tuple<Exploit, AssetGroup> group) {
    Exploit ex = get<0>(group);
    AssetGroup ag = get<1>(group);

    vector<int> perm = ag.perm;

    vector<ParameterizedQuality> param_postconds_q = ex.postcond_list_q();
    vector<ParameterizedTopology> param_postconds_t = ex.postcond_list_t();

    vector<Quality> postconds_q;
    vector<Topology> postconds_t;

    for(auto& postcond : param_postconds_q) {
        Quality q(perm[postcond.get_param_num()], postcond.get_name(), postcond.get_value());
        postconds_q.push_back(q);
    }

    for(auto& postcond : param_postconds_t) {
        Topology t(perm[postcond.get_from_param()], perm[postcond.get_to_param()], postcond.get_options());
    }

    return make_tuple(postconds_q, postconds_t);
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

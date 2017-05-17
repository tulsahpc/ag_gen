// ag_gen.cpp contains the methods for building an attack graph and generating an attack graph's exploits and
// printing them

#include <iostream>
#include <vector>

#include "ag_gen.h"
#include "network_state.h"

#include "util_common.h"
#include "util_odometer.h"

#ifdef DEBUG_BUILD
#define DEBUG(x) do { std::cerr << x << endl; } while (0)
#else
#define DEBUG(x)
#endif

using namespace std;

// The AGGen constructor creates a new AGGen object and takes the given NetworkState and sets it as the
// initial network state for generation by pushing it onto the new AGGen object's frontier vector.
AGGen::AGGen(const NetworkState& initial_state) :
        assets(Asset::fetch_all("home")),
        attrs(Quality::fetch_all_attributes()),
        vals(Quality::fetch_all_values()),
        current_state(const_cast<NetworkState&>(initial_state))
{
    this->frontier.push_back(initial_state);
}

// generate iterates through AGGen's frontier vector, back to front, and takes the next Network State, 
// generating all of the possible Network States that it could lead to based on its exploitable qualities
// and topologies. It then prints out the exploits of the new Network States.
void AGGen::generate(void) {
    vector<NetworkState> new_states;

    while(!this->frontier.empty()) {
        NetworkState next_state = this->frontier.back();

        this->frontier.pop_back();
        auto appl_exploits = check_exploits(next_state);

        // All of these exploits are applicable
        for (auto &e : appl_exploits) {
            auto postconditions = createPostConditions(e);

            auto qualities = get<0>(postconditions);
            auto topologies = get<1>(postconditions);

            NetworkState new_state = next_state;

            for (auto &qual : qualities) {
                new_state.get_factbase().add_quality(qual);
            }

            for (auto &topo : topologies) {
                new_state.get_factbase().add_topology(topo);
            }

            new_states.push_back(new_state);
//            this->frontier.push_back(new_state);
        }
    }

    for(auto& state : new_states) {
        state.print();
    }
}

// check_exploits takes a given NetworkState and returns a vector containing the exploits and corresponding
// asset groups that are applicable to the given NetworkState
vector<tuple<Exploit, AssetGroup> > AGGen::check_exploits(NetworkState& s) {
    vector<Exploit> exploit_list = Exploit::fetch_all();
    vector<tuple<Exploit, AssetGroup> > appl_exploit_list;

    for(auto& e : exploit_list) {
        vector<AssetGroup> asset_groups = gen_hypo_facts(s, e);
        for(auto asset_group : asset_groups) {
            // Each quality must exist. If not, discard asset_group entirely.
            bool applicable = check_assetgroup(s, asset_group);
            if (applicable) {
                appl_exploit_list.push_back(make_tuple(e, asset_group));
            }
        }
    }

    return appl_exploit_list;
}

// check_assetgroup takes a NetworkState and AssetGroup and returns a boolean. It returns true iff all of
// the hypothetical qualities and hypothetical topologies of the given AssetGroup are also part of the
// Network State's factbase
bool AGGen::check_assetgroup(NetworkState& s, AssetGroup& assetgroup) {
    for(auto& quality : assetgroup.hypothetical_qualities) {
        if(!s.get_factbase().find_quality(quality)) {
            return false;
        }
    }

    for(auto& topology : assetgroup.hypothetical_topologies) {
        if(!s.get_factbase().find_topology(topology)) {
            return false;
        }
    }

    return true;
}

// gen_hypo_facts takes a NetworkState and an Exploit, generates all the potential facts a Network State
// could have if the given exploit is effective by using all of the precondition qualities and topoligies
// of the exploit, and returns these in a vector of asset groups. This is used to check if an exploit
// applies or not
vector<AssetGroup> AGGen::gen_hypo_facts(NetworkState& s, Exploit& e) {
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

// createPostConditions takes a tuple of an Exploit and an AssetGroup. The function assumes the given
// exploit works, therefore it generates the postconditions (both the qualities and toplogies) that will be
// added to the factbase of a new state. It does this by iterating through the respective postcondition
// list of the given exploit and creates a new quality/topology from the current one and the given asset
// group's perm for each item in the list. It returns the postconditions as tuple of a vector of the new
// qualities and a vector of the new topologies.
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
}f

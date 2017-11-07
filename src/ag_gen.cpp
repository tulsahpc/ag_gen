// ag_gen.cpp contains the methods for building an attack graph and generating an attack graph's exploits and
// printing them

#include <iostream>
#include <vector>
#include <algorithm>

#include "ag_gen.h"
#include "util_odometer.h"
#include "util_db.h"
#include "edge.h"

using namespace std;

// The AGGen constructor creates a new AGGen object and takes the given NetworkState and sets it as the
// initial network state for generation by pushing it onto the new AGGen object's frontier vector.
AGGen::AGGen(NetworkState initial_state) {
    frontier.emplace_back(initial_state);
}

// generate iterates through AGGen's frontier vector, back to front, and takes the next Network State,
// generating all of the possible Network States that it could lead to based on its exploitable qualities
// and topologies. It then prints out the exploits of the new Network States.
void AGGen::generate() {
    auto counter = 0;
    while (!frontier.empty()) {
        cout << "Frontier Size: " << frontier.size() << endl;
        // Remove the next state from the queue and get its factbase
        auto current_state = frontier.front();
        frontier.pop_front();
		
        // Save the initial state's hash value
        // state_list[current_state.hash()] = current_state;

        // Get all applicable exploits with this network state
        auto appl_exploits = check_exploits(current_state);

        // Apply each exploit to the network state to generate new network states
        for (int i=0; i<appl_exploits.size(); i++) {
            auto e = appl_exploits.at(i);

            // For each applicable exploit, we extract which exploit applies and to which asset group it
            // applies to.
            auto exploit = get<0>(e);
            auto assetGroup = get<1>(e);

            // We generate the associated post conditions and extract the new qualities and topologies that
            // will be applied to the current factbase.
            auto postconditions = createPostConditions(e);
            auto qualities = get<0>(postconditions);
            auto topologies = get<1>(postconditions);

            // Deep copy the factbase so we can create a new network state
            NetworkState new_state {current_state};

            new_state.add_qualities(qualities);
            new_state.add_topologies(topologies);
			
			if(state_list.find(new_state.get_hash()) != state_list.end())
				continue;


			state_list[new_state.get_hash()] = new_state;
			frontier.emplace_front(new_state);
			counter++;

            // Save our new factbase to the database. Generate any new edges to the new network state from already
            // existing states.
//            try {
//                // If the factbase does not already exist, increment our number of new states and save the factbase
//                // to the database. Then we push the new network state onto the frontier. If the factbase does already
//                // exist, we create a new edge from the previous state to this one and move on.
//				if(hash_list.find(factbase.hash()) == hash_list.end()) {
//					counter++;
//					factbase.save();
//					NetworkState ns(factbase);
//					frontier.push_back(ns);
//				}
//
//				Edge edge(current_factbase.get_id(), factbase.get_id(), exploit, assetGroup);
//            } catch (DBException &e) {
//                cerr << e.what() << endl; // If theres an error, just print and quit abruptly.
//                abort();
//            }
        }
    }

    cout << "total number of generated states: " << counter << endl;
	cout << "states in state_list: " << state_list.size() << endl;
}

// Returns all applicable exploits to some given network state.
vector<tuple<Exploit, AssetGroup> > AGGen::check_exploits(const NetworkState &s) {
    vector<tuple<Exploit, AssetGroup> > appl_exploit_list;
    auto exploit_list = Exploit::fetch_all();

    for(int i=0; i<exploit_list.size(); i++) {
        auto e = exploit_list.at(i);
        auto asset_groups = gen_hypo_facts(s, e);
        for (auto asset_group : asset_groups) {
            // Each quality must exist. If not, discard asset_group entirely.
            auto applicable = check_assetgroup(s, asset_group);
            if (applicable) {
                appl_exploit_list.push_back(make_tuple(e, asset_group));
            }
        }
    }

    return appl_exploit_list;
}

// Generate all possible permutations with repetition of the asset bindings for the
// number of parameters needed by the exploit.
vector<AssetGroup> AGGen::gen_hypo_facts(const NetworkState &s, Exploit &e) {
    auto num_assets = s.get_num_assets();
    auto num_params = e.get_num_params();

    auto preconds_q = e.precond_list_q();
    auto preconds_t = e.precond_list_t();

    Odometer od(num_params, num_assets);
    vector<AssetGroup> asset_groups;

    for (auto j = 0; j < od.length(); j++) {
        auto perm = od.next();

        vector<Quality> asset_group_quals;
        vector<Topology> asset_group_topos;

        for (auto precond : preconds_q) {
            asset_group_quals.emplace_back(perm[precond.get_param_num()], precond.name, "=", precond.value);
        }

        for (auto precond : preconds_t) {
            auto dir = precond.get_dir();
            auto prop = precond.get_property();
            auto op = precond.get_operation();
            auto val = precond.get_value();

            asset_group_topos.emplace_back(perm[precond.get_from_param()], perm[precond.get_to_param()], dir, prop, op, val);
        }

        asset_groups.emplace_back(asset_group_quals, asset_group_topos, perm);
    }
    
    od.reset();

    return asset_groups;
}

// Check if an asset group binding works in the network state.
bool AGGen::check_assetgroup(const NetworkState &s, const AssetGroup &assetgroup) {
    for (auto quality : assetgroup.get_hypo_quals()) {
        if (!s.get_factbase().find_quality(quality)) {
            return false;
        }
    }

    for (auto topology : assetgroup.get_hypo_topos()) {
        if (!s.get_factbase().find_topology(topology)) {
            return false;
        }
    }

    return true;
}

// createPostConditions takes a tuple of an Exploit and an AssetGroup. The function assumes the given
// exploit works, therefore it generates the postconditions (both the qualities and toplogies) that will be
// added to the factbase of a new state. It does this by iterating through the respective postcondition
// list of the given exploit and creates a new quality/topology from the current one and the given asset
// group's perm for each item in the list. It returns the postconditions as tuple of a vector of the new
// qualities and a vector of the new topologies.
tuple<vector<Quality>, vector<Topology> > AGGen::createPostConditions(tuple<Exploit, AssetGroup> group) {
    auto ex = get<0>(group);
    auto ag = get<1>(group);

    auto perm = ag.get_perm();

    auto param_postconds_q = ex.postcond_list_q();
    auto param_postconds_t = ex.postcond_list_t();

    vector<Quality> postconds_q;
    vector<Topology> postconds_t;

    for (auto postcond : param_postconds_q) {
        Quality q(perm[postcond.get_param_num()], postcond.name, "=", postcond.value);
        postconds_q.push_back(q);
    }

    for (auto postcond : param_postconds_t) {
        auto dir = postcond.get_dir();
        auto prop = postcond.get_property();
        auto op = postcond.get_operation();
        auto val = postcond.get_value();

        Topology t(perm[postcond.get_from_param()], perm[postcond.get_to_param()], dir, prop, op, val);
        postconds_t.push_back(t);
    }

    return make_tuple(postconds_q, postconds_t);
}

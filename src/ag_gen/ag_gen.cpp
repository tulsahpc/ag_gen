// ag_gen.cpp contains the methods for building an attack graph and generating
// an attack graph's exploits and printing them

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "ag_gen.h"

#include "util/odometer.h"

using namespace std;

/**
 * @brief Constructor for generator
 * @details Builds a generator for creating attack graphs.
 *
 * @param net_i The network to build the attack graph for
 */
AGGen::AGGen(AGGenInstance &_instance) : instance(_instance), net(build_network(_instance)) {
    
    frontier.emplace_back(net.get_initial_state());
}

Network build_network(AGGenInstance &_instance)
{

    Network _net{_instance.opt_network, _instance.qualities, _instance.topologies, _instance.assets, _instance.facts};
    return _net;

}

/**
 * @brief Generates exploit postconditions
 * @details When an exploit is known to apply to a set of assets,
 * the postconditions must be generated. This is done by iterating
 * through each parameterized fact and inserting the applicable
 * assets.
 *
 * @param group A tuple containing the exploit and applicable assets
 * @return A tuple containing the "real" qualities and "real" topologies
 */
static std::tuple<std::vector<Quality>, std::vector<Topology>>
createPostConditions(std::tuple<Exploit, AssetGroup> &group) {
    auto ex = get<0>(group);
    auto ag = get<1>(group);

    auto perm = ag.get_perm();

    auto param_postconds_q = ex.postcond_list_q();
    auto param_postconds_t = ex.postcond_list_t();

    vector<Quality> postconds_q;
    vector<Topology> postconds_t;

    for (auto &postcond : param_postconds_q) {
        Quality q(perm[postcond.get_param_num()], postcond.name, "=",
                  postcond.value);
        postconds_q.push_back(q);
    }

    for (auto &postcond : param_postconds_t) {
        auto dir = postcond.get_dir();
        auto prop = postcond.get_property();
        auto op = postcond.get_operation();
        auto val = postcond.get_value();

        Topology t(perm[postcond.get_from_param()],
                   perm[postcond.get_to_param()], dir, prop, op, val);
        postconds_t.push_back(t);
    }

    return make_tuple(postconds_q, postconds_t);
}

/**
 * @brief Generate attack graph
 * @details Begin the generation of the attack graph. The algorithm is as follows:
 *
 *      1. Fetch next factbase to expand from the frontier
 *      2. Fetch all exploits
 *      3. Loop over each exploit to determine if it is applicable.
 *          a. Fetch preconditions of the exploit
 *          b. Generate all permutations of assets using the Odometer utility
 *          c. Apply each permutation of the assets to the preconditions.
 *          d. Check if ALL generated preconditions are present in the current factbase.
 *      4a. If all preconditions are found, apply the matching asset group to the postconditions of the exploit.
 *      4b. If not all preconditions are found, break and continue checking with the next exploit.
 *      5. Push the new network state onto the frontier to be expanded later.
 */
AGGenInstance& AGGen::generate() {

    std::vector<Exploit> exploit_list = instance.exploits;
    auto counter = 0;
    auto start = std::chrono::system_clock::now();

    //auto exploit_list = Exploit::fetch_all();
    unsigned long esize = exploit_list.size();

    cout << "Generating Attack Graph" << endl;
    while (!frontier.empty()) {
         cout << "Frontier Size: " << frontier.size() << endl;
        // Remove the next state from the queue and get its factbase
        auto current_state = frontier.front();
        frontier.pop_front();

        // Save the initial state's hash value
        // hash_list[current_state.hash()] = current_state;

        vector<tuple<Exploit, AssetGroup>> appl_exploits;

        // Get all applicable exploits with this network state
        for (int i = 0; i < esize; i++) {
            auto e = exploit_list.at(i);

            auto num_params = e.get_num_params();

            auto preconds_q = e.precond_list_q();
            auto preconds_t = e.precond_list_t();

            Odometer od(num_params, net.size());
            std::vector<AssetGroup> asset_groups;

            auto len = od.length();
            //for (auto j = 0; j < len; j++) {
            for (auto perm : od) {
                //auto perm = od[j];

                vector<Quality> asset_group_quals;
                vector<Topology> asset_group_topos;

                for (auto &precond : preconds_q) {
                    asset_group_quals.emplace_back(
                        perm[precond.get_param_num()], precond.name, "=",
                        precond.value);
                }

                for (auto &precond : preconds_t) {
                    auto dir = precond.get_dir();
                    auto prop = precond.get_property();
                    auto op = precond.get_operation();
                    auto val = precond.get_value();

                    asset_group_topos.emplace_back(
                        perm[precond.get_from_param()],
                        perm[precond.get_to_param()], dir, prop, op, val);
                }

                asset_groups.emplace_back(asset_group_quals, asset_group_topos,
                                          perm);
            }

            auto assetgroup_size = asset_groups.size();
            for (auto j = 0; j < assetgroup_size; j++) {
                auto asset_group = asset_groups.at(j);
                // Each quality must exist. If not, discard asset_group
                // entirely.
                for (auto &quality : asset_group.get_hypo_quals()) {
                    if (!current_state.get_factbase().find_quality(quality)) {
                        // continue;
                        goto LOOPCONTINUE;
                    }
                }

                for (auto &topology : asset_group.get_hypo_topos()) {
                    if (!current_state.get_factbase().find_topology(topology)) {
                        // continue;
                        goto LOOPCONTINUE;
                    }
                }
                {
                    auto new_appl_exploit = make_tuple(e, asset_group);
                    appl_exploits.push_back(new_appl_exploit);
                }
                LOOPCONTINUE:;
            }
        }

        // cout << "Applicable Exploits: " << appl_exploits.size() << endl;

        auto appl_expl_size = appl_exploits.size();

        // Apply each exploit to the network state to generate new network
        // states
        for (int j = 0; j < appl_expl_size; j++) {
            auto e = appl_exploits.at(j);

            // For each applicable exploit, we extract which exploit applies and
            // to which asset group it applies to.
            auto exploit = get<0>(e);
            auto assetGroup = get<1>(e);

            // We generate the associated post conditions and extract the new
            // qualities and topologies that will be applied to the current
            // factbase.
            auto postconditions = createPostConditions(e);
            auto qualities = get<0>(postconditions);
            auto topologies = get<1>(postconditions);

            instance.qualities.insert(instance.qualities.end(), qualities.begin(), qualities.end());
            instance.topologies.insert(instance.topologies.end(), topologies.begin(), topologies.end());

            // Deep copy the factbase so we can create a new network state
            NetworkState new_state{current_state};

            new_state.add_qualities(qualities);
            new_state.add_topologies(topologies);

            // ADD/UPDATE/DELETE code goes here

            // Store nodes in global list here

            auto res = hash_list.find(new_state.get_hash());

            //    If the factbase does not already exist, increment our
            //    number of new states and save the factbase to the
            //    database. Then we push the new network state onto the
            //    frontier. If the factbase does already exist, we create a
            //    new edge from the previous state to this one and move on.
            if (res != hash_list.end()) {
                // Edge edge(current_factbase.get_id(), factbase.get_id(),
                // exploit, assetGroup);
                continue;
            } else {
                hash_list.insert(new_state.get_hash());
                frontier.emplace_front(new_state);
                counter++;
            }
        }
    }

    cout << "Saving Attack Graph to Database" << endl;



    cout << "Completed" << endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout << "Total Time: " << elapsed_seconds.count() << " seconds" << endl;

    cout << "total number of generated states: " << counter << endl;
    cout << "states in hash_list: " << hash_list.size() << endl;

    return instance;
}

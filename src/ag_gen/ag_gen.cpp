// ag_gen.cpp contains the methods for building an attack graph and generating
// an attack graph's exploits and printing them

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>

#include "ag_gen.h"

#include "util/odometer.h"
#include "util/db_functions.h"

#ifdef REDIS
AGGen::~AGGen() {
    if (!use_redis)
        delete rman;
}

/**
 * @brief Constructor for generator
 * @details Builds a generator for creating attack graphs.
 *
 * @param _instance The initial information for generating the graph
 */
AGGen::AGGen(AGGenInstance &_instance, RedisManager &_rman) : instance(_instance), rman(&_rman) {
    rman->clear();
    auto init_quals = instance.initial_qualities;
    auto init_topos = instance.initial_topologies;
    NetworkState init_state(init_quals, init_topos);
    init_state.set_id();
    int init_id = init_state.get_id();
    FactbaseItems init_items =
                make_tuple(make_tuple(init_quals, init_topos), init_id);
    instance.factbases.push_back(init_state.get_factbase());
    instance.factbase_items.push_back(init_items);
    std::string hash = std::to_string(init_state.get_hash(instance.facts));
    // std::cout << "before init insertion" << std::endl;
    rman->insert_factbase(hash, init_id);
    // rman->insert_facts(hash, init_quals, init_topos);
    rman->commit();
    // std::cout << "after init insertion" << std::endl;
    // hash_map.insert(std::make_pair(init_state.get_hash(instance.facts), init_id));
    frontier.push_back(init_state);
    use_redis = true;
}
#endif

AGGen::AGGen(AGGenInstance &_instance) : instance(_instance) {
    auto init_quals = instance.initial_qualities;
    auto init_topos = instance.initial_topologies;
    NetworkState init_state(init_quals, init_topos);
    init_state.set_id();
    int init_id = init_state.get_id();
    FactbaseItems init_items =
                make_tuple(make_tuple(init_quals, init_topos), init_id);
    instance.factbases.push_back(init_state.get_factbase());
    instance.factbase_items.push_back(init_items);
    std::string hash = std::to_string(init_state.get_hash(instance.facts));
    hash_map.insert(std::make_pair(init_state.get_hash(instance.facts), init_id));
    frontier.push_back(init_state);
    use_redis = false;
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
static std::tuple<std::vector<std::tuple<ACTION_T, Quality>>, std::vector<std::tuple<ACTION_T, Topology>>>
createPostConditions(std::tuple<Exploit, AssetGroup> &group, Keyvalue &facts) {
    auto ex = std::get<0>(group);
    auto ag = std::get<1>(group);

    auto perm = ag.get_perm();

    auto param_postconds_q = ex.postcond_list_q();
    auto param_postconds_t = ex.postcond_list_t();

    std::vector<std::tuple<ACTION_T, Quality>> postconds_q;
    std::vector<std::tuple<ACTION_T, Topology>> postconds_t;

    for (auto &postcond : param_postconds_q) {
        auto action = std::get<0>(postcond);
        auto fact = std::get<1>(postcond);

        Quality q(perm[fact.get_param_num()], fact.name, "=",
                  fact.value, facts);
        postconds_q.push_back(std::make_tuple(action, q));
    }

    for (auto &postcond : param_postconds_t) {
        auto action = std::get<0>(postcond);
        auto fact = std::get<1>(postcond);

        auto dir = fact.get_dir();
        auto prop = fact.get_property();
        auto op = fact.get_operation();
        auto val = fact.get_value();

        Topology t(perm[fact.get_from_param()],
                   perm[fact.get_to_param()], dir, prop, op, val, facts);
        postconds_t.push_back(std::make_tuple(action, t));
    }

    return make_tuple(postconds_q, postconds_t);
}

/**
 * @brief Generate attack graph
 * @details Begin the generation of the attack graph. The algorithm is as
 * follows:
 *
 *      1. Fetch next factbase to expand from the frontier
 *      2. Fetch all exploits
 *      3. Loop over each exploit to determine if it is applicable.
 *          a. Fetch preconditions of the exploit
 *          b. Generate all permutations of assets using the Odometer utility
 *          c. Apply each permutation of the assets to the preconditions.
 *          d. Check if ALL generated preconditions are present in the current
 * factbase. 4a. If all preconditions are found, apply the matching asset group
 * to the postconditions of the exploit. 4b. If not all preconditions are found,
 * break and continue checking with the next exploit.
 *      5. Push the new network state onto the frontier to be expanded later.
 */
AGGenInstance &AGGen::generate(bool batch_process, int batch_size) {
    std::vector<Exploit> exploit_list = instance.exploits;
    auto counter = 0;
    auto start = std::chrono::system_clock::now();

    unsigned long esize = exploit_list.size();

    bool save_queued = false;

    std::cout << "Generating Attack Graph" << std::endl;

    std::unordered_map<size_t, PermSet<size_t>> od_map;
    size_t assets_size = instance.assets.size();
    for (auto ex : exploit_list) {
        size_t num_params = ex.get_num_params();
        if (od_map.find(num_params) == od_map.end()) {
            Odometer<size_t> od(num_params, assets_size);
            od_map[num_params] = od.get_all();
        }
    }

    while (!frontier.empty()) {
        if (batch_process && (save_queued || (counter+1) % batch_size == 0)) {
            save_ag_to_db(instance, false);
            std::cout << "done\n";
            instance.factbases.clear();
            instance.factbase_items.clear();
            instance.edges.clear();
            save_queued = false;
        }

//        cout << "Frontier Size: " << frontier.size() << endl;
        // Remove the next state from the queue and get its factbase
        auto current_state = frontier.back();
        auto current_hash = current_state.get_hash(instance.facts);
        frontier.pop_back();

//        std::cout << "Current State: " << current_state.get_id() << std::endl;

        std::vector<std::tuple<Exploit, AssetGroup>> appl_exploits;

        // std::cout << "Number of Exploits: " << esize << std::endl;
        // Get all applicable exploits with this network state
        for (size_t i = 0; i < esize; i++) {
            auto e = exploit_list.at(i);
            // std::cout << "Exploit: " << e.get_id() << std::endl;

             size_t num_params = e.get_num_params();
            // std::cout << "\tNum Params: " << num_params << std::endl;

            auto preconds_q = e.precond_list_q();
            // std::cout << "\tNum Precond Qualities: " << preconds_q.size() << std::endl;

            auto preconds_t = e.precond_list_t();
            // std::cout << "\tNum Precond Topologies: " << preconds_t.size() << std::endl << std::endl;

            // Odometer od(num_params, instance.assets.size());
            auto perms = od_map[num_params];
            std::vector<AssetGroup> asset_groups;

            for (auto perm : perms) {
                std::vector<Quality> asset_group_quals;
                std::vector<Topology> asset_group_topos;

                for (auto &precond : preconds_q) {
                    asset_group_quals.emplace_back(
                        perm[precond.get_param_num()], precond.name, "=",
                        precond.value, instance.facts);
//                    auto qual = Quality(perm[precond.get_param_num()], precond.name, "=", precond.value);
//                    qual.print();
                }

                for (auto &precond : preconds_t) {
                    auto dir = precond.get_dir();
                    auto prop = precond.get_property();
                    auto op = precond.get_operation();
                    auto val = precond.get_value();

                    asset_group_topos.emplace_back(
                        perm[precond.get_from_param()],
                        perm[precond.get_to_param()], dir, prop, op, val, instance.facts);
                }

                asset_groups.emplace_back(asset_group_quals, asset_group_topos,
                                          perm);
            }

            auto assetgroup_size = asset_groups.size();

            for (size_t j = 0; j < assetgroup_size; j++) {
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
                #pragma omp critical
                {
                    auto new_appl_exploit = std::make_tuple(e, asset_group);
                    appl_exploits.push_back(new_appl_exploit);
                }
            LOOPCONTINUE:;
            }
        }

        auto appl_expl_size = appl_exploits.size();

        // Apply each exploit to the network state to generate new network
        // states
        for (size_t j = 0; j < appl_expl_size; j++) {
            if (batch_process && (counter + 1) % batch_size == 0) save_queued = true;
            auto e = appl_exploits.at(j);

            // For each applicable exploit, we extract which exploit applies and
            // to which asset group it applies to.
            auto exploit = std::get<0>(e);
            auto assetGroup = std::get<1>(e);

            // We generate the associated post conditions and extract the new
            // qualities and topologies that will be applied to the current
            // factbase.
            auto postconditions = createPostConditions(e, instance.facts);
            auto qualities = std::get<0>(postconditions);
            auto topologies = std::get<1>(postconditions);

            // Deep copy the factbase so we can create a new network state
            NetworkState new_state{current_state};

            // ADD/UPDATE/DELETE code goes here
            for(auto &qual : qualities) {
                auto action = std::get<0>(qual);
                auto fact = std::get<1>(qual);

                switch(action) {
                case ADD_T:
                    new_state.add_quality(fact);
                    break;
                case UPDATE_T:
                    new_state.update_quality(fact);
                    break;
                case DELETE_T:
                    new_state.delete_quality(fact);
                    break;
                }
            }

            for(auto &topo : topologies) {
                auto action = std::get<0>(topo);
                auto fact = std::get<1>(topo);

                switch(action) {
                case ADD_T:
                    new_state.add_topology(fact);
                    break;
                case UPDATE_T:
                    new_state.update_topology(fact);
                    break;
                case DELETE_T:
                    new_state.delete_topology(fact);
                    break;
                }
            }

            auto hash_num = new_state.get_hash(instance.facts);

            if (hash_num == current_hash)
                continue;

            //    If the factbase does not already exist, increment our
            //    number of new states and save the factbase to the
            //    database. Then we push the new network state onto the
            //    frontier. If the factbase does already exist, we create a
            //    new edge from the previous state to this one and move on.
#ifdef REDIS
            if(use_redis) {
                auto hash = std::to_string(hash_num);
                if (!rman->check_factbase_exists(hash)) {
                    new_state.set_id();

                    auto facts_tuple = new_state.get_factbase().get_facts_tuple();
                    FactbaseItems new_items =
                        std::make_tuple(facts_tuple, new_state.get_id());
                    instance.factbase_items.push_back(new_items);

                    instance.factbases.push_back(new_state.get_factbase());
                    // std::cout << "before insertion" << std::endl;
                    rman->insert_factbase(hash, new_state.get_id());
                    // rman->insert_facts(hash, std::get<0>(facts_tuple), std::get<1>(facts_tuple));
                    rman->commit();
                    // std::cout << "after insertion" << std::endl;

                    // hash_map.insert(std::make_pair(new_state.get_hash(instance.facts), new_state.get_id()));

                    frontier.emplace_front(new_state);

                    Edge e(current_state.get_id(), new_state.get_id(), exploit, assetGroup);
                    e.set_id();

                    instance.edges.push_back(e);
                    counter++;
                } else {
                    int id = rman->get_factbase_id(hash);

                    Edge e(current_state.get_id(), id, exploit, assetGroup);
                    e.set_id();

                    instance.edges.push_back(e);
                }
            } else {
#endif // REDIS
                if (hash_map.find(hash_num) == hash_map.end()) {
                    new_state.set_id();

                    auto facts_tuple = new_state.get_factbase().get_facts_tuple();
                    FactbaseItems new_items =
                        std::make_tuple(facts_tuple, new_state.get_id());
                    instance.factbase_items.push_back(new_items);

                    instance.factbases.push_back(new_state.get_factbase());

                    hash_map.insert(std::make_pair(new_state.get_hash(instance.facts), new_state.get_id()));

                    frontier.emplace_front(new_state);

                    Edge e(current_state.get_id(), new_state.get_id(), exploit, assetGroup);
                    e.set_id();

                    instance.edges.push_back(e);
                    if (counter % 1000 == 0) std::cout << "State: " << counter << std::endl;
                    counter++;
                } else {
                    int id = hash_map[hash_num];

                    Edge e(current_state.get_id(), id, exploit, assetGroup);
                    e.set_id();
                    instance.edges.push_back(e);
                }
#ifdef REDIS
            }
#endif // REDIS
        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    instance.elapsed_seconds = elapsed_seconds;

    // std::cout << "Total Time: " << elapsed_seconds.count() << " seconds" << std::endl;
    // std::cout << "Generated States: " << counter << std::endl;

    return instance;
}

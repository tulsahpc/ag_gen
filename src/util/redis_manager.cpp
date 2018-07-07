
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cpp_redis/cpp_redis>

#include "ag_gen/quality.h"
#include "ag_gen/topology.h"
#include "redis_manager.h"

RedisManager::RedisManager(std::string host, int port,
                           std::vector<std::pair<std::string, std::string>> &sm) {
    client.connect(host, port);

    for (auto item : sm) {
        auto r = client.script_load(item.second);
        commit();
        script_map[item.first] = r.get().as_string();
    }
}

void RedisManager::insert_facts(std::string &hash, std::vector<Quality> &quals,
                                std::vector<Topology> &topos) {
    std::vector<std::string> strs;
    strs.resize(quals.size() + topos.size());

    std::transform(quals.begin(), quals.end(), strs.begin(),
                   [](Quality &q){ return std::to_string(q.get_encoding()); });

    std::transform(topos.begin(), topos.end(), std::next(strs.begin(), quals.size()),
                   [](Topology &t){ return std::to_string(t.get_encoding()); });

    client.sadd(hash + ":facts", strs);
}

// void RedisManager::insert_qualities(std::string &hash, std::vector<Quality> &quals) {
//     std::vector<std::string> strs;
//     strs.resize(quals.size());

//     std::transform(quals.begin(), quals.end(), strs.begin(),
//                    [](Quality q){ return std::to_string(q.get_encoding()); });

//     client.sadd(hash + ":qualities", strs);
// }

// void RedisManager::insert_topologies(std::string &hash, std::vector<Topology> &topos) {
//     std::vector<std::string> strs;
//     strs.resize(topos.size());

//     std::transform(topos.begin(), topos.end(), strs.begin(),
//                    [](Topology t){ return std::to_string(t.get_encoding()); });

//     client.sadd(hash + ":topologies", strs);
// }

void RedisManager::insert_factbase(std::string &hash, int id) {
    client.sadd("factbases", std::vector<std::string>{hash});
    client.hset("idmap:" + hash, "id", std::to_string(id));
}

std::string RedisManager::insert_collision_factbase(std::string &hash, int id) {
    // client.sadd("collisions", std::vector<std::string>{hash});
    // auto r = client.incr("count:" + hash);
    // commit();

    // int amt = r.get().as_integer();

    // std::string new_string = hash + "_" + std::to_string(amt);

    // client.hset("idmap:" + new_string, "id", std::to_string(id));

    // return new_string;

    auto r = client.evalsha(script_map["collisions"], 1,
                            std::vector<std::string>{hash}, std::vector<std::string>{std::to_string(id)});

    commit();

    return r.get().as_string();
}

void RedisManager::handle_collision(std::string &hash, int id, std::vector<Quality> &quals, std::vector<Topology> &topos) {
    std::string new_string = insert_collision_factbase(hash, id);

    insert_facts(new_string, quals, topos);
    commit();
}

int RedisManager::get_factbase_id(std::string &hash, int ci) {
    if (ci == 0 || ci == -1)
    {
        auto r = client.hget("idmap:" + hash, "id");
        commit();
        return std::stoi(r.get().as_string());
    } else {
        auto r = client.hget("idmap:" + hash + "_" + std::to_string(ci), "id");
        commit();
        return std::stoi(r.get().as_string());
    }
}

int RedisManager::get_collision_count(std::string &hash) {
    auto r = client.get("count:" + hash);
    commit();
    return std::stoi(r.get().as_string());
}

bool RedisManager::check_facts(std::string &hash, const std::vector<Quality> &quals,
                               const std::vector<Topology> &topos) {
    std::string sq = hash + ":facts";
    std::vector<std::string> encs;
    encs.resize(quals.size() + topos.size());

    std::transform(quals.begin(), quals.end(), encs.begin(),
                   [](Quality q){ return std::to_string(q.get_encoding()); });

    std::transform(topos.begin(), topos.end(), std::next(encs.begin(), quals.size()),
                   [](Topology t){ return std::to_string(t.get_encoding()); });

    bool checked = true;

    auto check_fact = [&](cpp_redis::reply reply) {
        if (reply.as_integer() != 1) checked = false;
    };

    std::for_each(encs.begin(), encs.end(),
                  [&](std::string enc){ client.sismember(sq, enc, check_fact); });

    commit();

    return checked;

}

bool RedisManager::check_factbase_exists(std::string &hash) {
    auto r = client.sismember("factbases", hash);
    commit();
    return r.get().as_integer() == 1;
}

bool RedisManager::check_collision(std::string &hash) {
    auto r = client.sismember("collisions", hash);
    commit();
    return r.get().as_integer() == 1;
}

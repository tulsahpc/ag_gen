
#ifndef REDIS_MANAGER_H
#define REDIS_MANAGER_H

#include <string>
#include <vector>
#include <cpp_redis/cpp_redis>

#include "ag_gen/quality.h"

class RedisManager {
    cpp_redis::client client;

    bool check_quality_exists(std::string s, size_t enc);
    bool check_topology_exists(std::string s, size_t enc);

    std::string insert_collision_factbase(std::string hash, int id);

  public:
    RedisManager(std::string host, int port);

    void insert_qualities(std::string hash, std::vector<Quality> &quals);
    void insert_topologies(std::string hash, std::vector<Topology> &topos);
    void insert_factbase(std::string hash, int id);

    void handle_collision(std::string hash, int id, std::vector<Quality> &quals, std::vector<Topology> &topos);

    int get_factbase_id(std::string hash, int ci);

    int get_collision_count(std::string hash);

    bool check_qualities(std::string hash, const std::vector<Quality> &quals);
    bool check_topologies(std::string hash, const std::vector<Topology> &topos);
    bool check_factbase_exists(std::string hash);

    bool check_collision(std::string hash);

    inline void commit() { client.sync_commit(); }

    inline void clear() { client.flushall(); commit(); }

};

#endif


#ifndef DB_FUNCTIONS_H
#define DB_FUNCTIONS_H

#include <vector>
#include <string>
#include <unordered_map>

#include "keyvalue.h"
#include "db.h"

#include "ag_gen/ag_gen.h"
#include "ag_gen/parameterized_quality.h"
#include "ag_gen/parameterized_topology.h"
#include "ag_gen/exploit.h"
#include "ag_gen/edge.h"
#include "ag_gen/quality.h"
#include "ag_gen/topology.h"
#include "ag_gen/asset.h"
#include "ag_gen/factbase.h"

void init_db(std::string connect_str);

std::vector<std::string> fetch_quality_attributes();
std::vector<std::string> fetch_quality_values();
std::vector<std::string> fetch_topology_attributes();
std::vector<std::string> fetch_topology_values();

std::unordered_map<
    int, std::tuple<std::vector<ParameterizedQuality>, std::vector<ParameterizedTopology>>>
               fetch_exploit_preconds();
std::unordered_map<
    int, std::tuple<std::vector<ParameterizedQuality>, std::vector<ParameterizedTopology>>>
               fetch_exploit_postconds();

std::vector<Exploit> fetch_all_exploits();
std::unordered_map<int, std::vector<Quality>> fetch_asset_qualities();
std::vector<Asset> fetch_all_assets(const std::string &network);
std::vector<Quality> fetch_all_qualities();
std::vector<Topology> fetch_all_topologies();
Keyvalue fetch_facts();

inline std::string to_query(Edge edge) { return edge.get_query(); }

void save_ag_to_db(std::vector<FactbaseItems> &factbase_items,
                   std::vector<Factbase> &factbases, std::vector<Edge> &edges,
                   Keyvalue &factlist);

#endif

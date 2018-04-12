#ifndef AG_GEN_HPP
#define AG_GEN_HPP

#include <deque>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "asset.h"
#include "assetgroup.h"
#include "edge.h"
#include "exploit.h"
#include "factbase.h"
#include "network.h"
#include "network_state.h"

#include "util/keyvalue.h"

struct AGGenInstance
{

	std::vector<Asset> assets;
	std::vector<Factbase> factbases;
	std::vector<Quality> qualities;
	std::vector<Topology> topologies;
	std::vector<Exploit> exploits;
	std::vector<Edge> edges;
	Keyvalue facts;

};

/** AGGen class
 * @brief Generate attack graph
 * @details Main generator class that stores state for the entire graph generation process.
 */
class AGGen {
	AGGenInstance instance;
    Network &net; //!< Network we are generating from
    std::deque<NetworkState> frontier; //!< Unexplored states
    std::set<size_t> hash_list{}; //!< List of hashes of known states
  public:
    explicit AGGen(Network &net_i);
    void generate(std::vector<Exploit> exploit_list);
};

#endif // AG_GEN_HPP

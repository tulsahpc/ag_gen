#ifndef AG_GEN_TOPOLOGY_H
#define AG_GEN_TOPOLOGY_H

#include <string>
#include <vector>

#include "util/keyvalue.h"

union EncodedTopology {
    struct {
        int from_asset : 16;
        int to_asset : 16;
        int dir : 2;
        int property : 10;
        int op : 4;
        int value : 16;

    } dec;
    size_t enc;
};

/** Topology class
 * @brief Holds information for the Topology
 */
class Topology {
    int from_asset_id;
    int to_asset_id;
    std::string property;
    std::string op;
    std::string value;
    std::string dir;

    friend class Factbase;

  public:
    Topology(int f_asset, int t_asset, std::string dir, std::string property,
             std::string op, std::string val);

    int get_from_asset_id() const;
    int get_to_asset_id() const;
    std::string get_property() const;
    std::string get_op() const;
    std::string get_value() const;
    std::string get_dir() const;

    void print() const;

    const EncodedTopology encode(const Keyvalue &kv_facts) const;

    bool operator==(const Topology &rhs) const;
    bool operator<(const Topology &rhs) const;
};

#endif // AG_GEN_TOPOLOGY_H

#ifndef AG_GEN_TOPOLOGY_H
#define AG_GEN_TOPOLOGY_H

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

class Topology {
    int from_asset_id;
    int to_asset_id;
    std::string property;
    std::string op;
    std::string value;
    std::string dir;

public:
    Topology(int f_asset, int t_asset, std::string &dir, std::string &property, std::string &op, std::string &val);

    explicit Topology(size_t fact);

    int get_from_asset_id() const;

    int get_to_asset_id() const;

    const std::string &getProperty() const;

    const std::string &getOp() const;

    const std::string &getValue() const;

    const std::string &getDir() const;

    void print() const;

    const EncodedTopology encode() const;

    bool operator==(const Topology &rhs) const;

    bool operator<(const Topology &rhs) const;

    static std::vector<Topology> fetch_all();

    static std::vector<std::string> fetch_all_attributes();

    static std::vector<std::string> fetch_all_values();
};

#endif //AG_GEN_TOPOLOGY_H

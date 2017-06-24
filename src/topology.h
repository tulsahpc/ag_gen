//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_TOPOLOGY_H
#define AG_GEN_TOPOLOGY_H

union EncodedTopology {
    struct {
        int from_asset : 32;
        int to_asset : 32;
		int options : 32;
    } dec;
    size_t enc;
};

class Topology {
    int from_asset_id;
    int to_asset_id;
    std::string options;

public:
    Topology(int f_asset, int t_asset, std::string opt);

    int get_from_asset_id(void) const;
    int get_to_asset_id(void) const;
    std::string get_raw_options(void) const;
	std::vector<std::string> get_options(void) const;

    void print(void) const;
    const EncodedTopology encode(void) const;
    bool operator==(const Topology& rhs) const;

    static std::vector<const Topology> fetch_all(void);
};

#endif //AG_GEN_TOPOLOGY_H

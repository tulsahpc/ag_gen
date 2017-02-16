#ifndef FACT_HPP
#define FACT_HPP

#include <iostream>

#include "asset.hpp"
#include "types.hpp"
#include "keyvalue.hpp"

class Asset;

union EncodedQuality {
	struct {
		int asset_id : 32;
		int attr : 16;
		int val : 16;
	} dec;
	int enc;
};

class Quality {
	int asset_id;
	std::string name;
    std::string value;

public:
    Quality(int assetId, std::string qualName, std::string qualValue);
    std::string get_name(void);

    void print(void);
    int encoded(void);
    bool operator==(const Quality& rhs);

    static std::vector<Quality> fetch_all(void);
    static std::vector<std::string> fetch_all_attributes(void);
    static std::vector<std::string> fetch_all_values(void);
};

class Topology {
	int from_asset_id;
	int to_asset_id;
	std::string options;

public:
	Topology(int f_asset, int t_asset, std::string opt);

	int get_from_asset_id(void);
	int get_to_asset_id(void);
	std::string get_options(void);
	bool operator==(const Topology& rhs);
	void print(void);

	static std::vector<Topology> fetch_all(void);
};

class ParameterizedQuality {
    int param;
    std::string name;
    std::string value;

public:
    ParameterizedQuality(int param, std::string attr, std::string val);
    void print(void);
    std::vector<Quality> make_quals(std::vector<int> asset_ids, Keyvalue<Asset>& assets);
    int get_param_num(void);
    std::string get_name(void);
    std::string get_value(void);
};

class ParameterizedTopology {
    int from_param;
    int to_param;
    std::string options;
public:
	ParameterizedTopology(int from_param, int to_param, std::string options);
	int get_from_param();
	int get_to_param();
	std::string get_options();

	void print();
};

struct AssetGroup {
	std::vector<Quality> hypothetical_qualities;
	std::vector<Topology> hypothetical_topologies;
};

#endif // FACT_HPP

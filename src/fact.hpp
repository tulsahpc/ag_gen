#ifndef FACT_HPP
#define FACT_HPP

#include <iostream>

#include "asset.hpp"
#include "types.hpp"

union EncodedQuality {
	struct {
		int asset_id : 32;
		int attr : 16;
		int val : 16;
	} dec;
	int enc;
};

class ParameterizedQuality {
    int param;
    std::string name;
    std::string value;
public:
    ParameterizedQuality(int param, std::string attr, std::string val);
    void print();
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

    static std::vector<Quality> fetch_all(void);
    static std::vector<std::string> fetch_all_attributes(void);
    static std::vector<std::string> fetch_all_values(void);
};

#endif // FACT_HPP

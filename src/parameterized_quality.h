//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_PARAMETERIZED_QUALITY_H
#define AG_GEN_PARAMETERIZED_QUALITY_H

#include <vector>
#include <string>

#include "asset.h"
#include "quality.h"
#include "keyvalue.h"

class ParameterizedQuality {
    int param;
    std::string name;
    std::string value;

public:
    ParameterizedQuality(int param, std::string attr, std::string val);

    int get_param_num(void);
    std::string get_name(void);
    std::string get_value(void);

    void print(void);

    std::vector<Quality> make_quals(std::vector<int> &asset_ids, Keyvalue<Asset> &assets);
};

#endif //AG_GEN_PARAMETERIZED_QUALITY_H

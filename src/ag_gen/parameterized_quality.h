//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_PARAMETERIZED_QUALITY_H
#define AG_GEN_PARAMETERIZED_QUALITY_H

#include <string>
#include <vector>

#include "asset.h"
#include "quality.h"
#include "util/keyvalue.h"

struct ParameterizedQuality {
    int param;
    std::string name;
    std::string value;

    int get_param_num();
    void print();
};

#endif // AG_GEN_PARAMETERIZED_QUALITY_H

//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_PARAMETERIZED_TOPOLOGY_H
#define AG_GEN_PARAMETERIZED_TOPOLOGY_H

#include <string>

class ParameterizedTopology {
    int from_param;
    int to_param;
    std::string options;
public:
    ParameterizedTopology(int from_param, int to_param, std::string options);
    int get_from_param(void);
    int get_to_param(void);
    std::string get_options(void);

    void print(void);
};

#endif //AG_GEN_PARAMETERIZED_TOPOLOGY_H

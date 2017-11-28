//
// Created by Kyle Cook on 4/9/17.
//

#ifndef AG_GEN_PARAMETERIZED_TOPOLOGY_H
#define AG_GEN_PARAMETERIZED_TOPOLOGY_H

#include <string>

class ParameterizedTopology {
    int from_param;
    int to_param;
    std::string dir;
    std::string prop;
    std::string op;
    std::string val;
public:
    ParameterizedTopology(int from_param, int to_param, std::string &dir, std::string &prop, std::string &op,
                          std::string &val);

    int get_from_param();

    int get_to_param();

    std::string get_dir();

    std::string get_property();

    std::string get_operation();

    std::string get_value();

    void print();
};

#endif //AG_GEN_PARAMETERIZED_TOPOLOGY_H

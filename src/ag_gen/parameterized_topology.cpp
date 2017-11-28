#include <iostream>

#include "parameterized_topology.h"

using namespace std;

ParameterizedTopology::ParameterizedTopology(int from_param_in, int to_param_in,
                                             std::string &dir,
                                             std::string &prop, std::string &op,
                                             std::string &val)
    : from_param(from_param_in), to_param(to_param_in), dir(dir), prop(prop),
      op(op), val(val) {}

int ParameterizedTopology::get_from_param() { return from_param; }

int ParameterizedTopology::get_to_param() { return to_param; }

std::string ParameterizedTopology::get_dir() { return dir; }

std::string ParameterizedTopology::get_property() { return prop; }

std::string ParameterizedTopology::get_operation() { return op; }

std::string ParameterizedTopology::get_value() { return val; }

void ParameterizedTopology::print() {
    cout << "From Param: " << to_string(from_param) << endl;
    cout << "To Param: " << to_string(to_param) << endl;
    cout << "Direction: " << dir << endl << endl;
    cout << "Property: " << prop << endl << endl;
    cout << "Operation: " << op << endl << endl;
    cout << "Value: " << val << endl << endl;
}

#include <iostream>

#include "parameterized_topology.h"

using namespace std;

int ParameterizedTopology::get_from_param() { return from_param; }
int ParameterizedTopology::get_to_param() { return to_param; }
std::string ParameterizedTopology::get_dir() { return dir; }
std::string ParameterizedTopology::get_property() { return prop; }
std::string ParameterizedTopology::get_operation() { return op; }
std::string ParameterizedTopology::get_value() { return val; }

/**
 * @brief Prints the Parametrized Topology
 */
void ParameterizedTopology::print() {
    cout << "From Param: " << to_string(from_param) << endl;
    cout << "To Param: " << to_string(to_param) << endl;
    cout << "Direction: " << dir << endl << endl;
    cout << "Property: " << prop << endl << endl;
    cout << "Operation: " << op << endl << endl;
    cout << "Value: " << val << endl << endl;
}

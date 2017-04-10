#include "fact.h"

using namespace std;

ParameterizedTopology::ParameterizedTopology(int from_param_in, int to_param_in, std::string options_in) :
        from_param(from_param_in), to_param(to_param_in), options(options_in) {}

int ParameterizedTopology::get_from_param() {
    return from_param;
}

int ParameterizedTopology::get_to_param() {
    return to_param;
}

std::string ParameterizedTopology::get_options() {
    return options;
}

void ParameterizedTopology::print() {
    cout << "From Param: " << to_string(from_param) << endl;
    cout << "To Param: " << to_string(to_param) << endl;
    cout << "Options: " << options << endl << endl;
}

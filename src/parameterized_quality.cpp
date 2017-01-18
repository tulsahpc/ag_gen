#include <iostream>
#include <string>
#include "fact.hpp"

using namespace std;

ParameterizedQuality::ParameterizedQuality(int param_in, string attr_in, string val_in) : param(param_in), name(attr_in), value(val_in) {}

void ParameterizedQuality::print() {
    cout << "Param: " + to_string(param) << endl;
    cout << "Attribute: " + name << endl;
    cout << "Value: " + value << endl << endl;
}

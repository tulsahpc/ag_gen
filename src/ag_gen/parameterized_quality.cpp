#include <iostream>

#include "parameterized_quality.h"

using namespace std;

void ParameterizedQuality::print() {
    cout << "Param: " + to_string(param) << endl;
    cout << "Attribute: " + name << endl;
    cout << "Value: " + value << endl << endl;
}

// Parameter numbers are 1-indexed
int ParameterizedQuality::get_param_num() { return param - 1; }

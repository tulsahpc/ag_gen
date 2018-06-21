#include <iostream>

#include "parameterized_quality.h"

using namespace std;

/**
 * @brief Prints Quality information.
 */
void ParameterizedQuality::print() {
    cout << "Param: " + to_string(param) << endl;
    cout << "Attribute: " + name << endl;
    cout << "Value: " + value << endl << endl;
}

/**
 * @brief Gets the current parameter number.
 * @details Parameter numbers are 1-indexed.
 * @return The current parameter number.
 */
int ParameterizedQuality::get_param_num() { return param; }

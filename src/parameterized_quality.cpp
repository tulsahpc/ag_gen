#include <iostream>

#include "parameterized_quality.h"

using namespace std;

ParameterizedQuality::ParameterizedQuality(int param_in, string attr_in, string val_in) : param(param_in), name(attr_in), value(val_in) {}

void ParameterizedQuality::print() {
    cout << "Param: " + to_string(param) << endl;
    cout << "Attribute: " + name << endl;
    cout << "Value: " + value << endl << endl;
}

vector<Quality> ParameterizedQuality::make_quals(vector<int> asset_ids, Keyvalue<Asset>& assets) {
    vector<Quality> new_quals;
    for(auto i=0; i<assets.length(); i++) {
        Quality q(asset_ids[i], name, value);
        new_quals.push_back(q);
    }
    return new_quals;
}

// Parameter numbers are 1-indexed
int ParameterizedQuality::get_param_num(void) {
    return param-1;
}

string ParameterizedQuality::get_name(void) {
    return name;
}

string ParameterizedQuality::get_value(void) {
    return value;
}

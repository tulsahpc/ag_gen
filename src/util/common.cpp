/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved.
 * Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "util/common.h"

using namespace std;

unsigned int base_convert_string(string data, int base) {
    unsigned int total = 0;
    for (char i : data) {
        unsigned int next_num = 0;
        if (i <= 'z' && i >= 'a') {
            next_num = (unsigned int)i - 87;
        } else if (i <= 'Z' && i >= 'A') {
            next_num = (unsigned int)i - 55;
        } else if (i <= '9' && i >= '0') {
            next_num = (unsigned int)i - '0';
        } else {
            printf("Malformed Input\n");
            exit(1);
        }
        total = total * base + next_num;
    }
    return total;
}

vector<int> base_convert_int(int num, int base) {
    vector<int> str;
    while (num > 0) {
        str.push_back(num % base);
        num = num / base;
    }

    for (int &i : str) {
        i = alphabet[i];
    }

    return str;
}

vector<int> base_convert(string num, int from, int to) {
    return base_convert_int(base_convert_string(std::move(num), from), to);
}

vector<string> split(string str, char delim) {
    vector<string> split_string;
    stringstream ss(str);
    string tmp;
    while (getline(ss, tmp, delim)) {
        split_string.push_back(tmp);
    }
    return split_string;
}

string trim(string str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(),
                                   not1(ptr_fun<int, int>(isspace))));
    return str;
}

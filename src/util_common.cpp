/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <cstdlib>
#include <vector>
#include <string>
#include <memory>

#include "util_common.hpp"

using namespace std;

unsigned int base_convert_string(string data, int base)
{
	int total = 0;
	for(int i=0; i<data.size(); i++) {
		unsigned int next_num = 0;
		if(data[i] <= 'z' && data[i] >= 'a') {
			next_num = data[i] - 87;
		} else if(data[i] <= 'Z' && data[i] >= 'A') {
			next_num = data[i] - 55;
		} else if(data[i] <= '9' && data[i] >= '0') {
			next_num = data[i] - '0';
		} else {
			printf("Malformed Input\n");
			exit(1);
		}
		total = total * base + next_num;
	}
	return total;
}

unique_ptr<vector<int> > base_convert_int(int num, int base)
{
	vector<int> str;
	while(num > 0) {
		str.push_back((unsigned long)(num % base));
		num = num / base;
	}


	for(int i=0; i<str.size(); i++) {
		str[i] = alphabet[str[i]];
	}

	return make_unique<vector<int> >(str);
}

unique_ptr<vector<int> > base_convert(string num, int from, int to)
{
	return base_convert_int(base_convert_string(num, from), to);
}

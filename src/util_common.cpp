/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "util_common.h"

using namespace std;

unsigned int base_convert_string(string data, int base)
{
	unsigned int total = 0;
	for(int i=0; i<data.size(); i++) {
		unsigned int next_num = 0;
		if(data[i] <= 'z' && data[i] >= 'a') {
			next_num = (unsigned int)data[i] - 87;
		} else if(data[i] <= 'Z' && data[i] >= 'A') {
			next_num = (unsigned int)data[i] - 55;
		} else if(data[i] <= '9' && data[i] >= '0') {
			next_num = (unsigned int)data[i] - '0';
		} else {
			printf("Malformed Input\n");
			exit(1);
		}
		total = total * base + next_num;
	}
	return total;
}

vector<int> base_convert_int(int num, int base)
{
	vector<int> str;
	while(num > 0) {
		str.push_back(num % base);
		num = num / base;
	}


	for(int i=0; i<str.size(); i++) {
		str[i] = alphabet[str[i]];
	}

	return str;
}

vector<int> base_convert(string num, int from, int to)
{
	return base_convert_int(base_convert_string(num, from), to);
}

vector<string> split(string str, char delim) {
	vector<string> split_string;
	stringstream ss(str);
	string tmp;
	while(getline(ss, tmp, delim)) {
		split_string.push_back(tmp);
	}
	return split_string;
};

string trim(string str) {
	str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace))));
	return str;
}

// Always free the string when no longer required
char *dynstr(const char *str)
{
	size_t str_len = strlen(str);
	char *new_str = (char *) malloc(sizeof(char)*(str_len+1));
	if(new_str == NULL)
		return (char *) -1;

	strncpy(new_str, str, str_len);
	return new_str;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include "config.hpp"

using namespace std;

unique_ptr<unordered_map<string, string> > read_config(void) {
	ifstream config_file("config.txt");

	if (!config_file.is_open()) {
		cout << "Unable to open file." << endl;
		return nullptr;
	} else {
		unordered_map<string, string> config;
		string next_line;
		while(getline(config_file, next_line)) {
			unique_ptr<vector<string> > splitStrPtr = split(next_line, ':');
			vector<string> splitStr = *splitStrPtr.get();
			for(int i=0; i<splitStr.size()-1; i+=2) {
				config[trim(splitStr[i])] = trim(splitStr[i+1]);
			}
		}

		config_file.close();
		return make_unique<unordered_map<string, string> >(config);
	}
}
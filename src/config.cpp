#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "config.h"
#include "util_common.h"

using namespace std;

// Config file:
//      - server
//      - db
//      - login
//      - password (optional)

Config::Config(const string &filename) {
	ifstream config_file(filename);

	if (!config_file.is_open()) {
		cout << "Unable to open config file." << endl;
        exit(1);
    } else {
        unordered_map<string, string> config;
		string next_line;
		while(getline(config_file, next_line)) {
			vector<string> splitStr = split(next_line, ':');
			for(int i=0; i<splitStr.size()-1; i+=2) {
				config[trim(splitStr[i])] = trim(splitStr[i+1]);
			}
		}

		config_file.close();
		this->config = unordered_map<string, string>(config);
	}
}

string Config::db_string(void) {
    string db_string;
    if(config["password"] == "")
    	db_string = "postgresql://" + config["login"] + "@" + config["server"] + "/" + config["db"];
    else
        db_string = "postgresql://" + config["login"] + ":" + config["password"] + "@" + config["server"] + "/" + config["db"];
	return db_string;
}

void Config::print(void) {
	for(const auto& it : config) {
		cout << it.first << ": " << it.second << endl;
	}
}

string Config::operator[](const std::string& q) {
		return config[q];
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ag_gen/config.h"
#include "util/common.h"

using namespace std;

// Config file:
//      - server
//      - db
//      - login
//      - password (optional)

/**
 * @brief Constructor for Config
 * @details Reads and parses config file and stores data
 *
 * @param filename Path of config file
 */
Config::Config(string filename) {
    ifstream config_file(filename);

    if (!config_file.is_open()) {
        cout << "Unable to open config file." << endl;
        exit(1);
    } else {
        unordered_map<string, string> config;
        string next_line;
        while (getline(config_file, next_line)) {
            vector<string> splitStr = split(next_line, ':');
            for (int i = 0; i < splitStr.size() - 1; i += 2) {
                config[trim(splitStr[i])] = trim(splitStr[i + 1]);
            }
        }

        config_file.close();
        this->config = unordered_map<string, string>(config);
    }
}

/**
 * @brief Builds a string for the database URI
 */
string Config::db_string() {
    string db_string;
    if (config["password"].empty())
        db_string = "postgresql://" + config["login"] + "@" + config["server"] +
                    ":" + config["port"] + "/" + config["db"];
    else
        db_string = "postgresql://" + config["login"] + ":" +
                    config["password"] + "@" + config["server"] + ":" +
                    config["port"] + "/" + config["db"];
    return db_string;
}

void Config::print() {
    for (const auto &it : config) {
        cout << it.first << ": " << it.second << endl;
    }
}

string Config::operator[](const std::string &q) { return config[q]; }

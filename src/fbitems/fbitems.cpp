
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "ag_gen/asset.h"
#include "ag_gen/quality.h"
#include "ag_gen/topology.h"

#include "util/db_functions.h"
#include "util/keyvalue.h"

void print_usage() {
    std::cout << "Usage: fbitems [OPTIONS...]" << std::endl
              << "\t-h\tShows this help menu." << std::endl
              << "\t-i\tindex. If no index provided, all items will be found." << std::endl
              << "\t-c\tconfig section. If none is provided, default will be used." << std::endl;
}

void find_one(std::vector<std::string> &str_vector, int index) {

    std::vector<std::pair<size_t, std::string>> fbitems;

    try {
        fbitems = fetch_one_factbase_items(index);
    } catch (CustomDBException &ex) {
        std::cout << index << " not in DB" << std::endl;
        return;
    }

    std::vector<Asset> assets = fetch_all_assets();

    std::cout << index << ":";

    for (auto item : fbitems) {

        std::string type = item.second;

        if (type == "quality") {

            std::cout << "\tquality:";

            EncodedQuality eq;
            eq.enc = item.first;
            Asset asset = assets[eq.dec.asset_id];
            std::cout << asset.get_name();

            std::string attr = str_vector[eq.dec.attr];
            std::string val = str_vector[eq.dec.val];

            std::cout << attr << "=" << val << "\n";

        } else if (type == "topology") {

            std::cout << "\ttopology:";

            EncodedTopology et;
            et.enc = item.first;

            Asset from_asset = assets[et.dec.from_asset];
            Asset to_asset = assets[et.dec.to_asset];

            std::cout << from_asset.get_name() << "->" << to_asset.get_name();

            std::string prop = str_vector[et.dec.property];
            std::string val = str_vector[et.dec.value];

            std::cout << prop << "=" << val << "\n";
        }
    }

    std::cout << std::endl;
}

void find_all(std::vector<std::string> &str_vector) {

    std::vector<std::vector<std::pair<size_t, std::string>>> fbitems;

    try {
        fbitems = fetch_all_factbase_items();
    } catch (CustomDBException &ex) {
        std::cout << "factbase_item TABLE empty" << std::endl;
        return;
    }

    std::vector<Asset> assets = fetch_all_assets();

    for (int i = 0; i < fbitems.size(); ++i) {

        auto items = fbitems[i];

        if (items.empty())
            continue;

        std::cout << i << ":";

        for (auto item : items) {

            std::string type = item.second;

            if (type == "quality") {

                std::cout << "\tquality:";

                EncodedQuality eq;
                eq.enc = item.first;
                Asset asset = assets[eq.dec.asset_id];
                std::cout << asset.get_name() << ",";

                std::string attr = str_vector[eq.dec.attr];
                std::string val = str_vector[eq.dec.val];

                std::cout << attr << "=" << val << "\n";

            } else if (type == "topology") {

                std::cout << "\ttopology:";

                EncodedTopology et;
                et.enc = item.first;

                Asset from_asset = assets[et.dec.from_asset];
                Asset to_asset = assets[et.dec.to_asset];

                std::cout << from_asset.get_name() << "->" << to_asset.get_name() << ",";

                std::string prop = str_vector[et.dec.property];
                std::string val = str_vector[et.dec.value];

                std::cout << prop << "=" << val << "\n";
            }
        }

        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {

    std::string opt_config;
    std::string opt_index;

    int opt;
    while ((opt = getopt(argc, argv, "hc:i:")) != -1) {
        switch (opt) {
        case 'c':
            opt_config = optarg;
            break;
        case 'i':
            opt_index = optarg;
            break;
        case 'h':
            print_usage();
            return 0;
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "wtf\n");
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "Unknown option -%c.\n", optopt);
            print_usage();
            exit(EXIT_FAILURE);
        }
    }

    std::string config_section = (opt_config.empty()) ? "default" : opt_config;

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);

    std::string host = pt.get<std::string>(config_section + ".host");
    std::string port = pt.get<std::string>(config_section + ".port");
    std::string dbName = pt.get<std::string>(config_section + ".db");
    std::string username = pt.get<std::string>(config_section + ".username");
    std::string password = pt.get<std::string>(config_section + ".password");

    init_db("postgresql://" + username + "@" + host + ":" + port + "/" + dbName);

    Keyvalue kv = fetch_facts();
    std::vector<std::string> str_vector = kv.get_str_vector();

    if (opt_index.empty()) {
        find_all(str_vector);
    } else {
        try {
            int index = boost::lexical_cast<int>(opt_index);
            find_one(str_vector, index);
        } catch(boost::bad_lexical_cast) {
            print_usage();
            return 2;
        }
    }

    // All done :)
    return 0;
}

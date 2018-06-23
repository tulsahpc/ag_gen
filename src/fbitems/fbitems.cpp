
#include <iostream>
#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

#include "ag_gen/quality.h"
#include "ag_gen/topology.h"
#include "ag_gen/asset.h"

#include "util/keyvalue.h"
#include "util/db_functions.h"

void print_usage(char *argv[])
{

    std::cout << "Usage: " << argv[0] << " (optional: index)" << std::endl
              << "\t If no index is provided, all items will be printed." << std::endl;

}

void find_one(std::vector<std::string> &str_vector, int index)
{

    std::vector<std::pair<size_t, std::string>> fbitems;

    try {
        fbitems = fetch_one_factbase_items(index);
    } catch (CustomDBException& ex) {
        std::cout << index << " not in DB" << std::endl;
        return;
    }

    std::vector<Asset> assets = fetch_all_assets();

    std::cout << index << ": ";

    for (auto item : fbitems)
    {

        std::string type = item.second;

        if (type == "quality")
        {

            std::cout << "\tquality:" << std::endl;

            EncodedQuality eq;
            eq.enc = item.first;
            Asset asset = assets[eq.dec.asset_id];
            std::cout << "\t\t" << asset.get_name() << std::endl;

            std::string attr = str_vector[eq.dec.attr];
            std::string val = str_vector[eq.dec.val];

            std::cout << "\t\t" << attr << "=" << val << std::endl;

        }
        else if (type == "topology")
        {

            std::cout << "\ttopoogy:" << std::endl;

            EncodedTopology et;
            et.enc = item.first;

            Asset from_asset = assets[et.dec.from_asset];
            Asset to_asset = assets[et.dec.to_asset];

            std::cout << "\t\tfrom:" << from_asset.get_name() << std::endl
                      << "\t\tto:" << to_asset.get_name() << std::endl;

            std::string prop = str_vector[et.dec.property];
            std::string val = str_vector[et.dec.value];

            std::cout << "\t\t" << prop << "=" << val << std::endl;

        }

    }

}

void find_all(std::vector<std::string> &str_vector)
{

    std::vector<std::vector<std::pair<size_t, std::string>>> fbitems;

    try {
        fbitems = fetch_all_factbase_items();
    } catch (CustomDBException& ex) {
        std::cout << "factbase_item TABLE empty" << std::endl;
        return;
    }

    std::vector<Asset> assets = fetch_all_assets();

    for (int i = 0; i < fbitems.size(); ++i)
    {

        std::cout << i << ": ";

        auto items = fbitems[i];

        for (auto item : items)
        {

            std::string type = item.second;

            if (type == "quality")
            {

                std::cout << "\tquality:" << std::endl;

                EncodedQuality eq;
                eq.enc = item.first;
                Asset asset = assets[eq.dec.asset_id];
                std::cout << "\t\t" << asset.get_name() << std::endl;

                std::string attr = str_vector[eq.dec.attr];
                std::string val = str_vector[eq.dec.val];

                std::cout << "\t\t" << attr << "=" << val << std::endl;

            }
            else if (type == "topology")
            {

                std::cout << "\ttopoogy:" << std::endl;

                EncodedTopology et;
                et.enc = item.first;

                Asset from_asset = assets[et.dec.from_asset];
                Asset to_asset = assets[et.dec.to_asset];

                std::cout << "\t\tfrom:" << from_asset.get_name() << std::endl
                          << "\t\tto:" << to_asset.get_name() << std::endl;

                std::string prop = str_vector[et.dec.property];
                std::string val = str_vector[et.dec.value];

                std::cout << "\t\t" << prop << "=" << val << std::endl;

            }

        }

    }

}

int main(int argc, char *argv[])
{

    init_db("postgresql://@localhost:5432/ag_gen");

    Keyvalue kv = fetch_facts();
    std::vector<std::string> str_vector = kv.get_str_vector();

    if (argc == 1)
        find_all(str_vector);
    else if (argc == 2)
    {

        try{
            int index = boost::lexical_cast<int>(argv[1]);
            find_one(str_vector, index);
        } catch(boost::bad_lexical_cast) {
            print_usage(argv);
            return 2;
        }

    }
    else
    {
        print_usage(argv);
        return 1;
    }

    // All done :)
    return 0;

}

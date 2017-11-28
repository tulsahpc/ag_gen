// main.cpp contains the main fuction that runs the program including flag handling and calls to functions
// that access the database and generate the attack graph.

#include <iostream>
#include <getopt.h>
#include <memory>

#include "ag_gen/ag_gen.h"
#include "ag_gen/config.h"

#include "util/db.h"

using namespace std;

std::shared_ptr<DB> db;

// print_usage prints to stdout the help menu that corresponds to the ag_gen command
void print_usage() {
    cout << "Usage: ag_gen [OPTION...]" << endl << endl;
    cout << "Flags:" << endl;
    cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
    cout << "\t-p\tPrint information about the network specified by -n." << endl;
    cout << "\t-h\tThis help menu." << endl;
}

// the main function executes the command according to the given flag and throws and error if an unknown flag
// is provided. It then uses the database given in the "config.txt" file to generate an attack graph.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    string opt_network;

    int opt;
    while ((opt = getopt(argc, argv, "hpn:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'n':
                opt_network = optarg;
                break;
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

    Config config("config.txt");
    db = std::make_shared<DB>(config.db_string());

    Network net {opt_network};
    AGGen gen {net};

    gen.generate();
}

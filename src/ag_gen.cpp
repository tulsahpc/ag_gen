#include <iostream>
#include <getopt.h>
#include <vector>
#include <memory>
#include <cstdlib>

#include "network.h"
#include "asset.h"
#include "exploit.h"

#include "util_db.h"

using namespace std;

#define CONNINFO "postgresql://localhost:5432/ag_gen_test"

static void print_usage()
{
	printf("Usage: ag_gen [OPTION...]\n");
	printf("\n");
	printf("Flags:\n");
	printf("\t-n\tNetwork model name to generate attack graph on.\n");
	printf("\t-p\tPrint information about the network specified by -n.\n");
	printf("\t-h\tThis help menu.\n");
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		print_usage();
		return 1;
	}

	int opt_print = 0;
	string opt_network;

	int c;
	while((c = getopt(argc, argv, "hpn:")) != -1) {
		switch(c) {
			case 'h':
				print_usage();
				return 0;
			case 'n':
				opt_network = optarg;
				break;
			case 'p':
				opt_print = 1;
				break;
			case '?':
				if(optopt == 'c')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				return EXIT_FAILURE;
				break;
			case ':':
				fprintf(stderr, "wtf\n");
				return EXIT_FAILURE;
				break;
			default:
				fprintf(stderr, "Unknown option -%c. Exiting.\n", optopt);
				return EXIT_FAILURE;
				break;
		}
	}

	dbconnect(CONNINFO);

	// try {
	// 	auto network = find_network("home");
	// 	cout << network->id << endl;
	// } catch (const exception &e) {
	// 	cout << e.what() << endl;
	// }

	vector<shared_ptr<Network> > network_list;
	networks_fetch(network_list);

	vector<shared_ptr<Asset> > asset_list;
	assets_fetch(asset_list, opt_network);

	vector<shared_ptr<Exploit> > exploit_list;
	exploits_fetch(exploit_list);

	for(auto network : network_list) {
		cout << network->name << endl;
	}

	for(auto asset : asset_list) {
		cout << asset->name << endl;
	}

	for(auto exploit : exploit_list) {
		cout << exploit->name << endl;
	}

	dbclose();
}

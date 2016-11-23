#include <iostream>
#include <getopt.h>
#include <memory>
#include <cstdlib>

#include "network.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "factbase.hpp"
#include "util_db.h"

using namespace std;

#define CONNINFO "postgresql://kyle@localhost/ag_gen_test"

class NetworkState {
	int id;
	int network_id;
	int parent_id;
	vector<Asset> assets;
public:
	NetworkState(Network &);
};

NetworkState::NetworkState(Network &net)
{
	id = net.id;
	network_id = net.id;
	parent_id = 0;
	assets = net.assets;
}

static void print_usage() {
    printf( "Usage: ag_gen [OPTION...]\n" );
    printf( "\n" );
    printf( "Flags:\n" );
    printf( "\t-n\tNetwork model name to generate attack graph on.\n" );
    printf( "\t-p\tPrint information about the network specified by -n.\n" );
    printf( "\t-h\tThis help menu.\n" );
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		print_usage();
		return 1;
	}

	int opt_print = 0;
	string opt_network;

	int opt;
	while((opt = getopt(argc, argv, "hpn:")) != -1) {
		switch(opt) {
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

	try {
		auto network = Network::find(opt_network);
	} catch (const exception &e) {
		cout << e.what() << endl;
	}

	vector<shared_ptr<Network> > network_list;
	Network::fetch_all(network_list);

	vector<shared_ptr<Asset> > asset_list;
	Asset::fetch_all(asset_list, opt_network);

	vector<shared_ptr<Exploit> > exploit_list;
	Exploit::fetch_all(exploit_list);

    cout << "Networks: " << endl;
	for(auto network : network_list) {
		cout << "\t" << network->name << endl;
	}
    cout << "Assets:" << endl;
	for(auto asset : asset_list) {
		cout << "\t" <<  asset->name << endl;
	}
    cout << "Exploits:" << endl;
	for(auto exploit : exploit_list) {
		cout << "\t" << exploit->name << endl;
	}

	dbclose();
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

#include <unordered_map>
#include <getopt.h>

#include "network.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "util_db.h"

using namespace std;

#define CONNINFO "postgresql://archlord@localhost/ag_gen_test"

unique_ptr<vector<string> > split(string str, char delim);
string trim(string str);
unique_ptr<unordered_map<string, string> > read_config(void);

class NetworkState {
	int id;
	int network_id;
	int parent_id;
public:
	NetworkState(Network &);
};

union TestQuality {
	struct {
		int asset : 8;
		int attr : 8;
		int val : 16;
	} dec;
	int enc;
};

enum assets {
	ROUTER, WORKSTATION, PRINTER
};

enum attributes {
	OS, VERSION, ROOT
};

enum values {
	WINDOWS, CISCO, HP, XP, THREE
};

NetworkState::NetworkState(Network &net)
{
	id = net.id;
	network_id = net.id;
	parent_id = 0;
}

static void print_usage() {
	cout << "Usage: ag_gen [OPTION...]" << endl << endl;
	cout << "Flags:" << endl;
	cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
	cout << "\t-p\tPrint information about the network specified by -n." << endl;
	cout << "\t-h\tThis help menu." << endl;
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		print_usage();
		return 0;
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
				exit(EXIT_FAILURE);
				break;
			case ':':
				fprintf(stderr, "wtf\n");
				exit(EXIT_FAILURE);
				break;
			default:
				fprintf(stderr, "Unknown option -%c. Exiting.\n", optopt);
				exit(EXIT_FAILURE);
				break;
		}
	}

	vector<TestQuality> factbase;
	TestQuality t1, t2, t3, t4, t5;
	t1.dec = {WORKSTATION,OS,WINDOWS};
	t2.dec = {WORKSTATION,VERSION,XP};
	t3.dec = {ROUTER,OS,CISCO};
	t4.dec = {ROUTER,VERSION,THREE};
	t5.dec = {PRINTER,OS,HP};

	printf("t1: %d\n", t1.enc);
	printf("t2: %d\n", t2.enc);
	printf("t3: %d\n", t3.enc);
	printf("t4: %d\n", t4.enc);
	printf("t5: %d\n", t5.enc);

	unique_ptr<unordered_map<string,string> > config = read_config();
	for(const auto &it : *config) {
		cout << it.first << ": " << it.second << endl;
	}
}

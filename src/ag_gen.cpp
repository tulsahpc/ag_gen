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
#include "util_odometer.h"

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
	ROUTER, WORKSTATION, PRINTER,
};

int asset_num = 3;

enum attributes {
	OS, VERSION, ROOT,
};

int attr_num = 3;

enum values {
	WINDOWS, CISCO, HP, XP, THREE,
};

int values_num = 5;

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

vector<int> gen_hypo_facts(void)
{
	vector<int> param_factbase;

	TestQuality t1,t2;
	t1.dec = {0,OS,WINDOWS};
	t2.dec = {0,VERSION,THREE};

	param_factbase.push_back(t1.enc);
	param_factbase.push_back(t2.enc);

	vector<int> hypo_factbase;

	Odometer od(1, 3);
	od.calc();

	for(int i=0; i<od.num_perms(); i++) {
		int *next_perm = od.next();
		if(next_perm == nullptr) {
			break;
		}

		for(int param : param_factbase) {
			TestQuality t;
			t.enc = param;
			if(t.dec.asset < od.perm_length()) {
				t.dec.asset = next_perm[t.dec.asset];
				hypo_factbase.push_back(t.enc);
			}
		}
	}

	return hypo_factbase;
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

	unique_ptr<unordered_map<string,string> > config = read_config();
	for(const auto &it : *config) {
		cout << it.first << ": " << it.second << endl;
	}
}

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
#include "util_common.hpp"
#include "util_db.hpp"
#include "util_odometer.hpp"
#include "keyvalue.hpp"


using namespace std;

#define CONNINFO "postgresql://archlord@localhost/ag_gen_test"

class AGGen {
public:
	AGGen(void);
};

static void print_usage()
{
    cout << "Usage: ag_gen [OPTION...]" << endl << endl;
    cout << "Flags:" << endl;
	cout << "\t-n\tNetwork model name to generate attack graph on." << endl;
	cout << "\t-p\tPrint information about the network specified by -n." << endl;
	cout << "\t-h\tThis help menu." << endl;
}

vector<int> gen_hypo_facts(void)
{
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

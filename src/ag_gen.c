/** \file ag_gen.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <getopt.h>

#include "ag_asset.h"
#include "ag_exploit.h"
#include "ag_network.h"
#include "db_util.h"
#include "util.h"
#include "util_list.h"
#include "util_odometer.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen"

void printUsage(void);

struct NetworkState {
	int network_id;
	struct NetworkState *parent;
	struct List *children;
};

static struct List *bindAssets(struct AGAssetList *assets, int *order, int len)
{
	struct List *assetBindings = ListNew();
	for(int i=0; i<len; i++) {
		ListAppend(assetBindings, (void*)AGAssetAt(assets, order[i]));
	}
	return assetBindings;
}

int main(int argc, char *argv[])
{
	int c;
	int opt_print = 0;
	char *opt_network = NULL;

	struct AGNetworkList *network_list;

	if(argc < 2) {
		printUsage();
		return 1;
	}

	while((c = getopt(argc, argv, "hpn:")) != -1) {
		switch(c) {
			case 'h':
				printUsage();
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
				exit(1);
				break;
			case ':':
				fprintf(stderr, "wtf\n");
				break;
			default:
				fprintf(stderr, "Unknown option -%c. Exiting.\n", optopt);
				exit(1);
				break;
		}
	}

	AGDbConnect(CONNINFO);

	network_list = AGGetNetworks();
	if(!network_list) return 1;

	if(network_list->len == 0)
		printf("Network does not exist or is empty.\n");

	struct AGAssetList *asset_list = AGGetAssets(opt_network);
	struct Odometer *od = OdometerNew(asset_list->len, 3);
	struct OdometerState *odst = initOdometerState(od);
	struct List *bindingList = ListNew();

	for(int j=0; j<od->size; j++) {
		int *nextPerm = nextPermutation(odst);
		struct List *boundAssets = bindAssets(asset_list, nextPerm, 3);
		ListAppend(bindingList, boundAssets);
	}

	for(int i=0; i<ListSize(bindingList); i++) {
		ListFree((ListGet(bindingList, i)));
	}

	ListFree(bindingList);
	free(odst);
	OdometerFree(od);

	AGAssetListFree(asset_list);
	AGNetworkListFree(network_list);

	AGDbDisconnect();
}

void printUsage()
{
	printf("Usage: ag_gen [OPTION...]\n");
	printf("\n");
	printf("Flags:\n");
	printf("\t-n\tNetwork model name to generate attack graph on.\n");
	printf("\t-p\tPrint information about the network specified by -n.\n");
	printf("\t-h\tThis help menu.\n");
}

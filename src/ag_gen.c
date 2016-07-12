/** \file ag_gen.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <getopt.h>

#include "ag_asset.h"
#include "ag_network.h"
#include "db_util.h"
#include "util.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen"

void printUsage(void);

int main(int argc, char *argv[])
{
	int print = 0;
	char *network = NULL;

	struct AGNetworkList *network_list;

	if(argc < 2) {
		printf("Not enough arguments. Use '-h' for the help menu.\n");
		return 1;
	}

	while((c = getopt(argc, argv, "hpn:")) != -1) {
		switch(c) {
		case 'h':
			printUsage();
			return 0;
		case 'n':
			network = optarg;
			break;
		case 'p':
			print = 1;
			break;
		case '?':
			if(optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			abort();
			break;
		default:
			abort();
			break;
		}
	}

	AGDbConnect(CONNINFO);

	network_list = AGGetNetworks();
	if(!network_list) return 1;

	if(network_list->len == 0)
		printf("Network does not exist or is empty.\n");

	AGNetworkListPrint(network_list);
	if(print) {
		struct AGAssetList *asset_list = AGGetAssets(network);
		AGAssetsPrint(asset_list);
		AGAssetListFree(asset_list);
	}

	AGNetworkListFree(network_list);
	AGDbDisconnect();
}

/**
 * Prints to the console the name and version of the attack graph generator
 *
 *
 */
void printUsage()
{
	printf("Usage: ag_gen [OPTION...]\n");
	printf("\n");
	printf("Flags:\n");
	printf("\t-n\tNetwork model name to generate attack graph on.\n");
	printf("\t-p\tPrint information about the network specified by -n.\n");
	printf("\t-h\tThis help menu.\n");
}

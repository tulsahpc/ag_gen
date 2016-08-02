/** \file ag_gen.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "asset.h"
#include "exploit.h"
#include "network.h"
#include "util_common.h"
#include "util_db.h"
#include "util_list.h"
#include "util_odometer.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen"

void printUsage(void);

struct NetworkState {
	int network_id;
	struct NetworkState *parent;
	struct List *children;
	struct List *assets;
};

static struct AGAsset **bind_assets(struct List *assets, int *order, int len)
{
	struct AGAsset **list = malloc(sizeof(struct AGAsset *) * len);
	for(int i=0; i<len; i++) {
		list[i] = list_at(assets, order[i]);
	}
	return list;
}

int main(int argc, char *argv[])
{
	int c;
	int opt_print = 0;
	char *opt_network = NULL;

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

	dbconnect(CONNINFO);

	struct List *network_list = networks_fetch();
	if(!network_list) {
		fprintf(stderr, "An error occurred retrieving the networks.\n");
		exit(1);
	}

	if(network_list->size == 0) {
		fprintf(stderr,"There are no networks in the system.\n");
		exit(1);
	}

	struct List *asset_list = assets_fetch(opt_network);
	if(asset_list->size == 0) {
		fprintf(stderr, "The network %s does not exist.\n", opt_network);
		exit(1);
	}

	int exploit_params = 3;
	struct Odometer *od = odometer_new(asset_list->size, exploit_params);
	struct OdometerState *odst = ostate_new(od);
	struct List *binding_list = list_new();

	// Generate asset bindings
	for(int i=0; i<od->size; i++) {
		int *nextPerm = ostate_next(odst);
		struct AGAsset **bound_assets = bind_assets(asset_list, nextPerm, exploit_params);
		list_push(binding_list, bound_assets);
	}

	// Check exploits

	// Print bindings
	// for(int i=0; i<ListSize(binding_list); i++) {
	// 	struct AGAsset **bound_assets = ListGet(binding_list, i);
	// 	for(int j=0; j<exploit_params; j++) {
	// 		printf("%s ", bound_assets[j]->name);
	// 	}
	// 	printf("\n");
	// }

	// Cleanup
	for(int i=0; i<list_size(binding_list); i++)
		free(list_at(binding_list, i));

	list_free(binding_list);

	for(int i=0; i<list_size(asset_list); i++)
		asset_free(list_at(asset_list, i));
	list_free(asset_list);

	for(int i=0; i<list_size(network_list); i++)
		network_free(list_at(network_list, i));
	list_free(network_list);

	ostate_free(odst);
	odometer_free(od);

	dbclose();
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

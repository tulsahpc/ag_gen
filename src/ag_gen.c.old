/** \file ag_gen.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "asset.h"
#include "exploit.h"
#include "network.h"
#include "network_state.h"
#include "util_common.h"
#include "util_db.h"
#include "util_hash.h"
#include "util_list.h"
#include "util_odometer.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen_test"

static int num_assets;
static struct List *network_state_queue;
static struct List *network_state_finished;

static void print_usage()
{
	printf("Usage: ag_gen [OPTION...]\n");
	printf("\n");
	printf("Flags:\n");
	printf("\t-n\tNetwork model name to generate attack graph on.\n");
	printf("\t-p\tPrint information about the network specified by -n.\n");
	printf("\t-h\tThis help menu.\n");
}

static int getbindings(struct List *bl, struct List *assets, struct NetworkState *state, struct Exploit *sploit)
{
	struct Odometer *od = odometer_new(num_assets, sploit->num_params);
	struct OdometerState *st = ostate_new(od);
	int num_bindings = od->size;

	for(int i=0; i<num_bindings; i++) {
		int *binding = ostate_next(st);
		struct Asset **new_binding = malloc(sizeof(struct Asset *) * sploit->num_params);

		for(int j=0; j<sploit->num_params; j++) {
			new_binding[j] = list_at(assets, binding[j]);
		}

		list_push(bl, new_binding);
	}

	ostate_free(st);
	odometer_free(od);

	return 0;
}

static struct NetworkState *check(struct Exploit *sploit, struct Asset **binding)
{
	struct List *props = sploit->properties;
	for(int i=0; i<sploit->num_params; i++) {
		struct Asset *next_asset = binding[i];

		// XXX: We are repeatedly checking every asset with the exploit. Can't
		// we cache this and only check for topologies associated with the exploit?
		for(int j=0; j<list_size(props); j++) {
			struct ExploitPrecond *next_prop = list_at(props, j);

			int sploit_param = next_prop->param;
			char *sploit_property = next_prop->property;
			char *sploit_value = next_prop->value;

			char *asset_value = hashtable_get(next_asset->facts, sploit_property);
			// printf("sploit property: %s\n", sploit_property);
			// printf("asset value: %s\n", asset_value);

			if(!asset_value) {
				printf("%s does not exist in %s\n", sploit_property, next_asset->name);
				continue;
			}

			if(strcmp(sploit_value, asset_value) == 0) {
				printf("%s vulnerable to %s\n", next_asset->name, sploit->name);
			}
		}

		// printf("\t%d %s %s\n", sploit_param, sploit_property, sploit_value);
	}

	return NULL;
}

static struct List *match(struct Exploit *sploit, struct List *bindings)
{
	struct List *new_states_list = list_new();

	for(int i=0; i<list_size(bindings); i++) {
		struct NetworkState *new_state = check(sploit, list_at(bindings, i));
		if(new_state != NULL) {
			list_rpush(new_states_list, new_state);
		}
	}

	return new_states_list;
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		print_usage();
		return 1;
	}

	int opt_print = 0;
	char *opt_network = NULL;

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

	// Fetch Networks
	struct List *network_list = list_new();
	int res = networks_fetch(network_list);
	int num_networks = res;

	if(res == -1) {
		fprintf(stderr, "An error occurred retrieving the networks.\n");
		return EXIT_FAILURE;
	}

	if(num_networks == 0) {
		fprintf(stderr,"There are no networks in the system.\n");
		return EXIT_SUCCESS;
	}

	// Fetch Assets
	struct List *asset_list = list_new();
	res = assets_fetch(asset_list, opt_network);
	num_assets = res;

	if(num_assets == 0) {
		fprintf(stderr, "The network %s does not have any assets.\n", opt_network);
		return EXIT_SUCCESS;
	}

	// Fetch Exploits
	struct List *exploit_list = list_new();
	res = exploits_fetch(exploit_list);
	int num_exploits = res;

	if(res == 0) {
		fprintf(stderr, "No exploits in database.\n");
		return EXIT_SUCCESS;
	}

	network_state_queue = list_new();
	network_state_finished = list_new();

	struct NetworkState *initial_ns = ns_new(1, NULL);
	list_rpush(network_state_queue, initial_ns);

	// Main Attack Graph Loop
	while(!list_empty(network_state_queue)) {
		struct NetworkState *next_state = list_pop(network_state_queue);

		for(int i=0; i<num_exploits; i++) {
			struct Exploit *sploit = list_at(exploit_list, i);
			struct List *bindings = list_new();

			// XXX: BAD, can be pulled out of the loop later.
			getbindings(bindings, asset_list, next_state, sploit);
			struct List *new_states = match(sploit, bindings);

			// Merge new_states into queue
			for(int i=0; i<list_size(new_states); i++) {
				list_push(network_state_queue, list_at(new_states, i));
			}
			list_free(new_states);

			// Print bindings
			// for(int j=0; j<list_size(bindings); j++) {
			// 	struct Asset **next_binding = list_at(bindings, j);
			// 	for(int k=0; k<sploit->num_params; k++) {
			// 		printf("%s ", next_binding[k]->name);
			// 	}
			// 	printf("\n");
			// }

			// Cleanup
			list_iterate(bindings, free);
			list_free(bindings);

			list_rpush(network_state_finished, next_state);
		}
	}

	// Cleanup
	list_iterate(network_list, network_free);
	list_iterate(asset_list, asset_free);
	list_iterate(exploit_list, exploit_free);

	list_free(network_list);
	list_free(asset_list);
	list_free(exploit_list);

	dbclose();
}

#include <stdlib.h>
#include <getopt.h>

#include "ag_asset.h"
#include "db_util.h"
#include "util.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen"

void printUsage(void);

int main(int argc, char *argv[])
{
	int c;
	char *network = NULL;

	if(argc < 2) {
		printUsage();
		return 1;
	}

	while((c = getopt(argc, argv, "n:")) != -1) {
		switch(c) {
		case 'n':
			network = optarg;
			break;
		case '?':
			if(optopt == 'c') {
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				printUsage();
			}
			return 1;
		default:
			abort();
		}
	}

	AGDbConnect(CONNINFO);

	struct AGAssetList *list = AGGetAssets(network);
	DEBUG_PRINT("length: %d\n", list->len);

	if(list == NULL)
		return 1;

	if(list->len == 0) {
		printf("Network does not exist or is empty.\n");

		return 0;
	}

	AGAssetListFree(list);
	AGDbDisconnect();
}

void printUsage()
{
	printf("------------------------------------\n");
	printf("Attack Graph Generator\n");
	printf("Version 0.0.1\n");
	printf("------------------------------------\n");
	printf("\n");
	printf("\t-n, --network\t\tNetwork to generate attack graph.\n");
	printf("\n");
}

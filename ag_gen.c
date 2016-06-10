/**  \file
 *
 *
 *
 */
#include <stdlib.h>
#include "db_util.h"

#define CONNINFO "postgresql://localhost:5432/ag_gen"

void printUsage(void);

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printUsage();
		exit(1);
	}

	AGDbConnect(CONNINFO);
	AGDbDisconnect();
}

/**
 * Prints to the console the name and version of the attack graph generator
 *
 *
 */
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

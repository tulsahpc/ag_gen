/** \file ag_gen.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
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

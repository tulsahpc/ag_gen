/** \file db_test.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>

#include "network.h"
#include "asset.h"
#include "exploit.h"
#include "util_db.h"
#include "util_list.h"

// Database connection information
#define CONNINFO "postgresql://localhost:5432/ag_gen"

int main()
{
	struct List *network_list;
	struct List *asset_list;
	struct List *exploit_list;

	dbconnect(CONNINFO);

	network_list = networks_fetch();
	asset_list = assets_fetch("home");
	exploit_list = exploits_fetch();

	for(int i=0; i<asset_list->size; i++) {
		asset_free(list_at(asset_list, i));
	}
	list_free(asset_list);

	for(int i=0; i<exploit_list->size; i++) {
		exploit_free(list_at(exploit_list, i));
	}
	list_free(exploit_list);

	for(int i=0; i<network_list->size; i++) {
		network_free(list_at(network_list, i));
	}
	list_free(network_list);

	dbclose();
}

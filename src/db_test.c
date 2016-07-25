/** \file db_test.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>

#include "ag_network.h"
#include "ag_asset.h"
#include "ag_exploit.h"
#include "db_util.h"

// Database connection information
#define CONNINFO "postgresql://localhost:5432/ag_gen"

int main()
{
	struct AGNetworkList *networkList;
	struct AGAssetList *assetList;
	struct AGExploitList *exploitList;

	AGDbConnect(CONNINFO);

	networkList = AGGetNetworks();
	AGNetworkListPrint(networkList);
	AGNetworkListFree(networkList);

	assetList = AGGetAssets("home");
	AGAssetsPrint(assetList);
	AGAssetListFree(assetList);

	exploitList = AGGetExploits();
	AGExploitListPrint(exploitList);
	AGExploitListFree(exploitList);

	AGDbDisconnect();
}

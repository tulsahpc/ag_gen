#include "ag_network.h"
#include "ag_asset.h"
#include "db_util.h"

// Database connection information
#define CONNINFO "postgresql://localhost:5432/ag_gen"

int main()
{
    struct AGNetworkList *networkList;
    struct AGAssetList *assetList;
    struct AGNetwork *first;

    AGDbConnect(CONNINFO);

    networkList = AGGetNetworks();
    AGNetworksPrint(networkList);
    AGNetworksFree(networkList);

    first = networkList->networks[0];

    assetList = AGGetAssets("home");
    AGAssetsPrint(assetList);
    AGAssetsFree(assetList);

    AGDbDisconnect();
}

#include "ag_network.h"
#include "ag_asset.h"
#include "db_util.h"

// Database connection information
#define CONNINFO "postgresql://localhost:5432/ag_gen"

int main() {
    AGDbConnect(CONNINFO);

    struct AGNetworkList *networkList = AGGetNetworks();
    AGNetworksPrint(networkList);
    AGNetworksFree(networkList);

    struct AGAssetList *assetList = AGGetAssets();
    AGAssetsPrint(assetList);
    AGAssetsFree(assetList);

    AGDbDisconnect();
}

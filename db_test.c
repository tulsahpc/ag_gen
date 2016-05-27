#include "ag_network.h"
#include "ag_asset.h"
#include "db_util.h"

// Database connection information
#define CONNINFO "postgresql://localhost:5432/ag_gen"

int main() {
    AGDbConnect(CONNINFO);

    struct AGNetworkList *networkList = AGGetNetworks();
    printNetworks(networkList);
    AGFreeNetworks(networkList);

    struct AGAssetList *assetList = AGGetAssets();
    printAssets(assetList);
    AGFreeAssets(assetList);

    AGDbDisconnect();
}

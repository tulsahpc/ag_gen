/** \file ag_asset.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <memory>
#include <vector>

struct Quality {
	int asset_id;
	std::string property;
	std::string value;
};

struct Asset {
	int id;
	std::string name;
	int network_id;
};

struct AssetBinding {
	int len;
	struct Asset *assets;
};

int assets_fetch(std::vector<std::shared_ptr<Asset> > &, std::string const &);

#endif //ASSET_H

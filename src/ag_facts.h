/** \file ag_facts.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef C_AG_FACTS_H
#define C_AG_FACTS_H

#include "ag_asset.h"
#include "util_hash.h"

enum Property {
	OS
};

char *property_strings[] = {
	"os"
};

enum Attribute {
	TRUSTED,
	CONNECTED
};

char *attribute_strings[] = {
	"trusted",
	"connected"
};

struct AGPropertyList {
	enum Property **properties;
	int len;
};

struct AGAttributeList {
	enum Attribute **attributes;
	int len;
};

// Uniqueness defined by (asset_id, property)
struct AGQuality {
	int asset_id;
	struct HashTable *hash;
};

// Uniqueness defined by (from, to)
struct AGTopology {
	int asset_from;
	int asset_to;
	struct AGAttributeList *attributes;
};

union AGFactList {
	union AGFact **facts;
	int len;
};

union AGFact {
	struct AGQuality *quality;
	struct AGTopology *topology;
};

struct AGQuality *AGQualityNew(struct AGAsset *asset);

void AGSetProperty(struct AGQuality *quality, enum Property prop, char *val);
char *AGGetProperty(struct AGQuality *quality, enum Property prop);

#endif //C_AG_FACTS_H

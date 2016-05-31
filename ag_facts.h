//
// Created by Kyle Cook on 5/27/16.
//

#ifndef C_AG_FACTS_H
#define C_AG_FACTS_H

enum PROPERTY { OS };
enum ATTRIBUTE { TRUSTED, CONNECTED };

struct AGPropertyList {
	enum PROPERTY **properties;
	int len;
};

struct AGAttributeList {
	enum ATTRIBUTE **attributes;
	int len;
};

// Uniqueness defined by (asset_id, property)
struct AGQuality {
	int asset_id;
	struct AGPropertyList *properties;
	char *value;
};

// Uniqueness defined by (from, to)
struct AGTopology {
	int from;
	int to;
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

#endif //C_AG_FACTS_H

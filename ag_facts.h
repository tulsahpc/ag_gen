#ifndef C_AG_FACTS_H
#define C_AG_FACTS_H

/**
 * Enum of the different properties.
 *
 * Examples include operating system.
 */
enum PROPERTY { OS };

/**
 * What attributes the fact has.
 * Trusted for a device it trusts.
 * Connected for a device it is connected to.
 */
enum ATTRIBUTE { TRUSTED, CONNECTED };

/**
 * Struct of the list of properties.
 *
 * Has as double pointer as the list of properties and an int for the length of that list.
 */
struct AGPropertyList {
	enum PROPERTY **properties;
	int len;
};

/**
 * Struct of the list of attributes.
 *
 * Has a double point as the list of attributes and an int for the length of that list.
 */
struct AGAttributeList {
	enum ATTRIBUTE **attributes;
	int len;
};

/**
 *
 *
 *
 */
// Uniqueness defined by (asset_id, property)
struct AGQuality {
	int asset_id;
	struct AGPropertyList *properties;
	char *value;
};

/**
 *
 *
 *
 */
// Uniqueness defined by (from, to)
struct AGTopology {
	int from;
	int to;
	struct AGAttributeList *attributes;
};

/**
 *
 *
 *
 */
union AGFactList {
	union AGFact **facts;
	int len;
};

/**
 *
 *
 *
 */
union AGFact {
	struct AGQuality *quality;
	struct AGTopology *topology;
};

#endif //C_AG_FACTS_H

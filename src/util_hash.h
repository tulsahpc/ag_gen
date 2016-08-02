#ifndef UTIL_HASH_H
#define UTIL_HASH_H

#include <stdint.h>

struct HashNode {
	struct HashNode *prev;
	struct HashNode *next;
	char *key;
	void *val;
};

struct HashTable {
	int size;
	struct HashNode **arr;
};

static char alphabet[35] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'o',
	'p', 'q', 'r', 's',
	't', 'u', 'v', 'w',
	'x', 'u', 'z'
};

struct HashTable *hashtable_new(void);
void *hashtable_get(struct HashTable *table, char *key);
void hashtable_set(struct HashTable *table, char *key, void *val);
void hashtable_free(struct HashTable *table);

#endif //UTIL_HASH_H

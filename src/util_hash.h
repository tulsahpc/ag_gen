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
	unsigned int (*hash_func)(char *key);
	void (*key_destroy)(void *);
	void (*val_destroy)(void *);
};

struct HashTable *hashtable_new(unsigned int (*func)(char *key));
struct HashTable *hashtable_new_full(unsigned int (*func)(char *key), void (*key_destroy)(void *), void (*val_destroy)(void *));
void *hashtable_get(struct HashTable *table, char *key);
void hashtable_set(struct HashTable *table, char *key, void *val);
void hashtable_free(struct HashTable *table);

#endif //UTIL_HASH_H

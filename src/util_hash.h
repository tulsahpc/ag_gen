#ifndef C_UTIL_HASH_H
#define C_UTIL_HASH_H

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

struct HashTable *HashTableNew(void);
void HashTableAdd(struct HashTable *table, char *key, void *val);
void HashTableRemove(struct HashTable *table, char *key);
void *HashTableFind(struct HashTable *table, char *key);
void HashTableFree(struct HashTable *table);

// unsigned int base_convert_string(char *, int);
// char *base_convert_int(int, int);
char *base_convert(char *num, int from, int to);
uint64_t radix128(char *data);
unsigned int hash(char *key);

#endif //C_UTIL_HASH_H

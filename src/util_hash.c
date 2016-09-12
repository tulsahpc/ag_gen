#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util_common.h"
#include "util_list.h"
#include "util_hash.h"

#define HASH_PRIME 101

static uint64_t radix128(char *data)
{
	int len = strlen(data);
	int total = 0;
	for(int i=0; i<len; i++) {
		total = total*128 + data[i];
	}
	return total;
}

unsigned int radix_hash(char *key)
{
	uint64_t val = radix128(key) % HASH_PRIME;
	return val;
}

struct HashTable *hashtable_new(unsigned int (*func)(char *key))
{
	struct HashTable *new_table = calloc(1, sizeof(struct HashTable));
	new_table->size = HASH_PRIME;
	new_table->arr = calloc(1, new_table->size * sizeof(struct HashNode *));
	new_table->hash_func = func;
	return new_table;
}

struct HashTable *hashtable_new_full(unsigned int (*func)(char *key), void (*key_destroy)(void *), void (*val_destroy)(void*))
{

}

void *hashtable_get(struct HashTable *table, char *key)
{
	int hashed_key = table->hash_func(key);

	struct HashNode *cur = table->arr[hashed_key];
	int status = 1;
	while(cur) {
		if(strcmp(key, cur->key) == 0) {
			status = 0;
			break;
		}
		cur = cur->next;
	}

	if(status == 1)
		return NULL;

	return cur->val;
}

void hashtable_set(struct HashTable *table, char *key, void *val)
{
	int hashed_key = table->hash_func(key);
	struct HashNode *cur = table->arr[hashed_key];
	struct HashNode *new = calloc(1, sizeof(struct HashNode));

	if(!cur) {
		new->key = key;
		new->val = val;

		table->arr[hashed_key] = new;
	} else {
		new->next = cur;
		new->key = key;
		new->val = val;

		cur->prev = new;
		cur->next = NULL;

		table->arr[hashed_key] = new;
	}
}

void hashtable_free(struct HashTable *table)
{
	for(int i=0; i<table->size; i++) {
		struct HashNode *cur = table->arr[i];
		struct HashNode *del;
		while(cur) {
			del = cur;
			cur = cur->next;
			free(del);
		}
	}
	free(table->arr);
	free(table);
}

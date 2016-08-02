#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util_common.h"
#include "util_list.h"
#include "util_hash.h"

#define HASH_PRIME 101

static unsigned int base_convert_string(char *data, int base)
{
	int len = strlen(data);
	int total = 0;
	for(int i=0; i<len; i++) {
		unsigned int next_num = 0;
		if(data[i] <= 'z' && data[i] >= 'a') {
			next_num = data[i] - 87;
		} else if(data[i] <= 'Z' && data[i] >= 'A') {
			next_num = data[i] - 55;
		} else if(data[i] <= '9' && data[i] >= '0') {
			next_num = data[i] - '0';
		} else {
			printf("Malformed Input\n");
			exit(1);
		}
		total = total * base + next_num;
	}
	return total;
}

static char *base_convert_int(int num, int base)
{
	struct List st;
	while(num > 0) {
		list_push(&st, (void *)(unsigned long)(num % base));
		num = num / base;
	}

	int size = list_size(&st);
	char *converted = malloc(size * sizeof(char) + 1);
	for(int i=0; i<size; i++) {
		converted[i] = alphabet[(int)list_pop(&st)];
	}
	converted[size] = '\0';

	return converted;
}

static char *base_convert(char *num, int from, int to)
{
	return base_convert_int(base_convert_string(num, from), to);
}

static uint64_t radix128(char *data)
{
	int len = strlen(data);
	int total = 0;
	for(int i=0; i<len; i++) {
		total = total*128 + data[i];
	}
	return total;
}

static unsigned int hash(char *key)
{
	uint64_t val = radix128(key) % HASH_PRIME;
	return val;
}

struct HashTable *hashtable_new()
{
	struct HashTable *new_table = malloc(sizeof(struct HashTable));
	new_table->size = HASH_PRIME;
	new_table->arr = calloc(1, new_table->size * sizeof(struct HashNode *));
	return new_table;
}

void *hashtable_get(struct HashTable *table, char *key)
{
	int hashed_key = hash(key);
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
	int hashed_key = hash(key);
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

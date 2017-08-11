#ifndef _UTIL_HASH
#define _UTIL_HASH

#include <stdbool.h>
#include <stdint.h>

#define LOAD_FACTOR .75

struct hashtable {
    char** arr;
    int size;
    int used;
};
typedef struct hashtable hashtable;

uint64_t hash(char* str);

hashtable* new_hashtable(int size);
void init_hashtable(hashtable* t, int size);
void add_hashtable(hashtable* t, char* str);
void free_hashtable(hashtable* t);

double get_loadfactor(hashtable* t);
bool should_rehash(hashtable* t);

#endif

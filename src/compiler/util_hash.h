#ifndef _UTIL_HASH
#define _UTIL_HASH

#include <stdbool.h>
#include <stdint.h>

#define HASH_INIT_SIZE 100
#define LOAD_FACTOR .75

struct hashtable {
    char* arr;
    int size;
    int used;
};
typedef struct hashtable hashtable;

double get_loadfactor(hashtable* t);
bool should_rehash(hashtable* t);
uint64_t hash(char* str);
hashtable* new_hashtable(void);
void init_hashtable(hashtable* t);

#endif

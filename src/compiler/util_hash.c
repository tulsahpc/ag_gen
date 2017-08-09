#include <string.h>
#include "util_hash.h"
#include "str_util.h"

/* http://www.cse.yorku.ca/~oz/hash.html */
uint64_t hash(char* str) {
    uint64_t hash = 5381;
    int c;
    while((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// Load factor should be below 75%
double get_loadfactor(hashtable* t) {
    return ((double)t->used / t->size);
}

bool should_rehash(hashtable* t) {
    if(get_loadfactor(t) < LOAD_FACTOR)
        return true;
    else
        return false;
}

hashtable* new_hashtable(void) {
    hashtable* t = (hashtable*) getmem(sizeof(hashtable));
    init_hashtable(t);
    return t;
}

void init_hashtable(hashtable* t) {
    t->arr = (char*) getmem(HASH_INIT_SIZE*sizeof(char));
    t->size = HASH_INIT_SIZE;
    t->used = 0;
}

void add_hashtable(hashtable* t, char* str) {
    uint64_t idx = hash(str);
    
}

#include <string.h>
#include <stdlib.h>
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

void clear_hashtable(hashtable* t) {
    for(int i=0; i<t->size; i++) {
        t->arr[i] = 0;
    }
}

void rehash(hashtable* t) {
    hashtable* new = new_hashtable(t->size*2); 
    for(int i=0; i<t->size; i++) {
        if(t->arr[i] != 0) {
            add_hashtable(new, t->arr[i]);
        }
    }
    t = new;
}

hashtable* new_hashtable(int size) {
    hashtable* t = (hashtable*) getmem(sizeof(hashtable));
    init_hashtable(t, size);
    return t;
}

void init_hashtable(hashtable* t, int size) {
    t->arr = (char**) getmem(size*sizeof(char*));
    t->size = size;
    t->used = 0;

    clear_hashtable(t);
}

void add_hashtable(hashtable* t, char* str) {
    if(should_rehash(t))
        rehash(t);
    uint64_t idx = hash(str) % t->size;
    t->arr[idx] = str;
}

char* get_hashtable(hashtable* t, char* str) {
    uint64_t idx = hash(str) % t->size;
    return t->arr[idx];
}

void free_hashtable(hashtable* t) {
    for(int i=0; i<t->size; i++) {
        if(t->arr[i] != 0) {
            free(t->arr[i]);
        }
    }
}

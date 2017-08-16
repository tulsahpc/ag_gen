#include <stdio.h>
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

hashtable* new_hashtable(int size) {
    hashtable* t = (hashtable*) getmem(sizeof(hashtable));
    init_hashtable(t, size);
    return t;
}

void init_hashtable(hashtable* t, int size) {
    t->arr = (hashnode*) getmem(size*sizeof(hashnode));
    t->size = size;
    t->used = 0;

    clear_hashtable(t);
}

void clear_hashtable(hashtable* t) {
    for(int i=0; i<t->size; i++) {
        t->arr[i] = 0;
    }
}

void add_hashtable(hashtable* t, char* key, void* val) {
    // if(should_rehash(t))
        // rehash(t);
    uint64_t idx = hash(key) % t->size;
    printf("hashing: %s\n", key);

    hashnode* prevnode = t->arr[idx];
    while(prevnode != NULL)
        prevnode = prevnode->next;
    
    hashnode* newnode = (hashnode*) getmem(sizeof(hashnode))
    newnode->key = key;
    newnode->val = val;
    newnode->next = NULL;
    
    t->arr[idx] = newnode;
    t->used++;
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

// Load factor should be below 75%
double get_loadfactor(hashtable* t) {
    return ((double)t->used / t->size);
}

bool should_rehash(hashtable* t) {
    if(get_loadfactor(t) < LOAD_FACTOR)
        return false;
    else
        return true;
}

// void rehash(hashtable* t) {
//     printf("REHASHING\n");
//     t->arr = realloc(t->arr, ((t->size*2)+1)*sizeof(char*));
//     t->size *= 2;
// }

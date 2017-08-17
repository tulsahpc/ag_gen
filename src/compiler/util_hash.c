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
    t->arr = (hashnode**) getmem(size*sizeof(hashnode*));
    t->size = size;
    t->used = 0;
    clearmem(t->arr, size*sizeof(hashnode*));
}

void add_hashtable(hashtable* t, char* key, void* val) {
    // if(should_rehash(t))
        // rehash(t);
    uint64_t idx = hash(key) % t->size;
    // printf("hashing: %s\n", key);

    hashnode* newnode = (hashnode*) getmem(sizeof(hashnode));
    newnode->key = key;
    newnode->val = val;
    newnode->next = NULL;

    hashnode* prevnode = t->arr[idx];
    if(prevnode != NULL) {
        while(prevnode->next != NULL) {
            if(strcmp(prevnode->key, key) == 0) {
                free(newnode);
                return;
            }
            prevnode = prevnode->next;
        }
        
        prevnode->next = newnode;
    }
    
    t->arr[idx] = newnode;
    t->used++;
}

void* get_hashtable(hashtable* t, char* str) {
    uint64_t idx = hash(str) % t->size;
    hashnode* currnode = t->arr[idx];
    while(currnode != NULL) {
        if(strcmp(currnode->key, str) == 0)
            return currnode->val;
        currnode = currnode->next;
    }
    return (void*) -1;
}

void free_hashtable(hashtable* t) {
    for(int i=0; i<t->size; i++) {
        hashnode* node = t->arr[i];
        if(node != NULL) {
            while(node->next != NULL) {
                hashnode* tmp = node;
                free(node);
                node = tmp->next;
            }
            free(node);
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

/*
 * identifier.c
 *
 *  Created on: Jan 5, 2016
 *      Author: john-hale
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "identifier.h"

//STRAIGHT OUTTA SEDGEWICK 'ALGORITHMS IN C', p.233
// M should be prime and is defined in identifier.h
// 64 is a reasonable value based on alphabet size
unsigned int identifier_hash(char *v) {
    unsigned int h;
    for (h = 0; *v != '\0'; v++)
        h = (64 * h + *v) % M_id;
    return h;
}

// make and populate a new identifier structure
struct identifier *new_identifier(char *str, int type) {
    struct identifier *i = (struct identifier*)malloc(sizeof(struct identifier));
    i->key = str;
    i->type = type;
    return i;
}

// insert a new identifier structure into an identifier hashtable unless it already exists
struct identifier_hashtable_node *insert_identifier(char *str, int type, struct identifier_hashtable *i_ht) {
    struct identifier_hashtable_node *i_node = NULL;
    unsigned int index = identifier_hash(str);
    // see if an identifier already exists
    // if an identifier does not exist, create it and return the node containing it.
    // if the identifier already exists, return the node containing it.
    if ((i_node = find_identifier(str, type, i_ht)) == NULL) {
        struct identifier *id = new_identifier(str, type);
        i_node = (struct identifier_hashtable_node *) malloc(sizeof(struct identifier_hashtable_node));
        i_node->i = id;
        i_node->next = i_ht->head[index];
        i_ht->head[index] = i_node;
    }
    return i_node;
}

// insert a new identifier structure into an identifier hashtable unless it already exists
struct identifier *insert_and_get_identifier(char *str, int type, struct identifier_hashtable *i_ht) {
    struct identifier_hashtable_node *i_node = NULL;
    unsigned int index = identifier_hash(str);
    // see if an identifier already exists
    // if an identifier does not exist, create it and return the node containing it.
    // if the identifier already exists, return the node containing it.
    if ((i_node = find_identifier(str, type, i_ht)) == NULL) {
        struct identifier *id = new_identifier(str, type);
        i_node = (struct identifier_hashtable_node *) malloc(sizeof(struct identifier_hashtable_node));
        i_node->i = id;
        i_node->next = i_ht->head[index];
        i_ht->head[index] = i_node;
    }
    return i_node->i;
}

// locate an identifier in an identifier hashtable or return NULL trying
struct identifier_hashtable_node *find_identifier(char *key, int type, struct identifier_hashtable *i_ht) {
    struct identifier_hashtable_node *current_node;
    int done = 0;
    unsigned int index = identifier_hash(key);
    current_node = i_ht->head[index];
    while (!done) {
//		printf("in while loop for insert_identifier\n");
        if (current_node == NULL) {/*printf("current_node for identifier is NULL\n");*/ done = 1;
            break;
        }
        if (type == current_node->i->type && !strcmp(key, current_node->i->key)) {
            done = 1;
            break;
        }
        else current_node = current_node->next;
    }
//	printf("outside while loop for insert_identifier\n");
    return current_node;
}

struct identifier_hashtable_node *add_new_identifier(char *str, int type, struct identifier_hashtable *i_ht) {
    struct identifier_hashtable_node *i_node = NULL;
    struct identifier *id = new_identifier(str, type);
    unsigned int index = identifier_hash(str);
    i_node = (struct identifier_hashtable_node *) malloc(sizeof(struct identifier_hashtable_node));
    i_node->i = id;
    i_node->next = i_ht->head[index];
    i_ht->head[index] = i_node;
    return i_node;
}

// make an empty identifier hashtable
struct identifier_hashtable *make_identifier_hashtable(struct identifier_hashtable *i_ht, int id_m) {
    i_ht->size = id_m;
    i_ht->head = (struct identifier_hashtable_node **) calloc(id_m, id_m * sizeof(struct identifier_hashtable_node *));
    return i_ht;
}

char *identifier_hashtable_to_string(struct identifier_hashtable *i_ht) {
    int i;
    char tmp_str[30];
    char *str = "";
    struct identifier_hashtable_node *current_pointer;
//	printf("Identifier Hashtable: %p - head: %p - size:%d\n",&i_ht,i_ht->head, i_ht->size);
    for (i = 0; i < i_ht->size; i++) {
        asprintf(&str, "%s[%d] = %p\n", str, i, (void *) i_ht->head[i]);
        current_pointer = i_ht->head[i];
        while (current_pointer != NULL) {
            asprintf(&str, "%s--%s\n", str, current_pointer->i->key);
            current_pointer = current_pointer->next;
        }
    }
    return str;
}

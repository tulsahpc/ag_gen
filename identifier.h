/*
 * identifier.h
 *
 *  Created on: Jan 5, 2016
 *      Author: john-hale
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#define M_id 101

#define ASSET 1001
#define PROPERTY 1002
#define VALUE 1003
#define RELATIONSHIP 1004
#define PARAM 1005
#define XPLOIT 1006

struct identifier {
    char *key;
    int type;
};

struct identifier_hashtable {
    int size;
    struct identifier_hashtable_node **head;
};

struct identifier_hashtable_node {
    struct identifier *i;
    struct identifier_hashtable_node *next;
};

unsigned int identifier_hash(char *);

struct identifier *new_identifier(char *, int);

struct identifier_hashtable_node *add_new_identifier(char *, int, struct identifier_hashtable *);

struct identifier_hashtable_node *insert_identifier(char *, int, struct identifier_hashtable *);

struct identifier *insert_and_get_identifier(char *, int, struct identifier_hashtable *);

struct identifier_hashtable_node *find_identifier(char *, int, struct identifier_hashtable *);

struct identifier_hashtable *make_identifier_hashtable(struct identifier_hashtable *, int);

char *identifier_hashtable_to_string(struct identifier_hashtable *);

#endif
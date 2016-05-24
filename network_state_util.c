/*
 * network_state_util.c
 *
 *  Created on: Jan 4, 2016
 *      Author: john-hale
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "identifier.h"
#include "network_state.h"
#include "globals.h"

//STRAIGHT OUTTA SEDGEWICK 'ALGORITHMS IN C', p.233
// a_M should be prime and is defined in network_state.h
// 64 is a reasonable value based on alphabet size
unsigned int asset_hash(char *v) {
    unsigned int h;
    for (h = 0; *v != '\0'; v++)
        h = (64 * h + *v) % M_a;
//	printf("hash is %d\n",h);
    return h;
}

// make and populate a new asset structure
struct asset *new_asset(char *str, struct identifier_hashtable *i_ht) {
    struct asset *a = (struct asset *) malloc(sizeof(struct asset));
//	printf("Before call to insert_identifier\n");
    struct identifier_hashtable_node *i_node = insert_identifier(str, ASSET, i_ht);
//	printf("After call to insert_identifier\n");
    a->name = i_node->i;
    return a;
}

// make an empty asset hashtable, return it AND assign it to asset_ht
struct asset_hashtable *make_asset_hashtable(struct asset_hashtable *asset_ht) {
//	printf("in make_asset_hashtable\n");
    struct asset_hashtable *a_ht = (struct asset_hashtable *) malloc(sizeof(struct asset_hashtable));
    a_ht->size = M_a;
    a_ht->head = (struct asset_hashtable_node **) calloc(M_a, M_a * sizeof(struct asset_hashtable_node *));
    asset_ht = a_ht;
//	printf("Asset Hashtable freshly baked:\n%s",asset_hashtable_to_string(a_ht));
    return a_ht;
}

// locate an asset in an asset hashtable or return NULL trying
struct asset_hashtable_node *find_asset(char *key, struct asset_hashtable *a_ht) {
//	printf("in find_asset\n");
    struct asset_hashtable_node *current_node;
    int done = 0;
    unsigned int index = asset_hash(key);
    current_node = a_ht->head[index];
//	printf("just before loop\n");
    while (!done) {
//		printf("just inside loop\n");
        if (current_node == NULL) {/*printf("current_node for asset is NULL\n");*/ done = 1;
            break;
        }
        if (!strcmp(key, current_node->a->name->key)) {/*printf("strings match!\n");*/ done = 1;
            break;
        }
        else current_node = current_node->next;
//		printf("in loop\n");
    }
//	printf("out of while loop\n");
    return current_node;
}

// insert an asset structure into an asset hashtable unless it already exists
// return it in any case
struct asset_hashtable *insert_asset(char *str, struct asset_hashtable *a_ht, struct identifier_hashtable *i_ht) {
    struct asset_hashtable_node *a_node = NULL;
    unsigned int index = asset_hash(str);
//	printf("in insert_asset = %s; hash = %u\n",str,index);
    // see if an asset already exists
    // if an asset does not exist, create it and return the node containing it.
    // if the asset already exists, return the node containing it.
    if ((a_node = find_asset(str, a_ht)) == NULL) {
//		printf("a_node is NULL\n");
        struct asset *a = new_asset(str, i_ht);
        a_node = (struct asset_hashtable_node *) malloc(sizeof(struct asset_hashtable_node));
        a_node->a = a;
        a_node->next = a_ht->head[index];
        a_ht->head[index] = a_node;
    }
//	printf("Escaped condition.\n");
    return a_ht;
}

// insert an asset structure into an asset hashtable unless it already exists
// return it in any case
struct asset_hashtable_node *insert_and_get_asset(char *str, struct asset_hashtable *a_ht,
                                                  struct identifier_hashtable *i_ht) {
    struct asset_hashtable_node *a_node = NULL;
    unsigned int index = asset_hash(str);
    // see if an asset already exists
    // if an asset does not exist, create it and return the node containing it.
    // if the asset already exists, return the node containing it.
    if ((a_node = find_asset(str, a_ht)) == NULL) {
        struct asset *a = new_asset(str, i_ht);
        a_node = (struct asset_hashtable_node *) malloc(sizeof(struct asset_hashtable_node));
        a_node->a = a;
        a_node->next = a_ht->head[index];
        a_ht->head[index] = a_node;
    }
    return a_node;
}

char *asset_hashtable_to_string(struct asset_hashtable *a_ht) {
    int i;
    char tmp_str[30];
    char *str = "";
    struct asset_hashtable_node *current_pointer;
    asprintf(&str, "Asset Hashtable: %p - head: %p - size:%d\n", &a_ht, a_ht->head, a_ht->size);
    for (i = 0; i < a_ht->size; i++) {
//		asprintf(&str,"%s[%d] = %p\n",str,i,(void *) a_ht->head[i]);
        current_pointer = a_ht->head[i];
        while (current_pointer != NULL) {
            asprintf(&str, "%s--%s\n", str, current_pointer->a->name->key);
            current_pointer = current_pointer->next;
        }
    }
    return str;
}

unsigned int fact_hash(struct fact *f) {
    unsigned int h;
    if (f->type == QUAL) h = quality_hash(f->q->a->name->key, f->q->property->key);
    else h = topology_hash(f->t->a1->name->key, f->t->a2->name->key, f->t->relationship->key);
    return h;
}

//ADAPTED FROM SEDGEWICK 'ALGORITHMS IN C', p.233
// M_f should be prime and is defined in network_state.h
// 64 is a reasonable value based on alphabet size
unsigned int quality_hash(char *v1, char *v2) {
    unsigned int h;
    char *v;
    asprintf(&v, "%s%s", v1, v2);
//	printf("quality_hash: v1 is %s, v2 is %s, v is %s\n",v1,v2,v);
    for (h = 0; *v != '\0'; v++)
        h = (64 * h + *v) % M_f;
//	printf("hash is %d\n",h);
    return h;
}

//ADAPTED FROM SEDGEWICK 'ALGORITHMS IN C', p.233
// f_M should be prime and is defined in network_state.h
// 64 is a reasonable value based on alphabet size
unsigned int topology_hash(char *v1, char *v2, char *v3) {
    unsigned int h;
    char *v;
    asprintf(&v, "%s%s%s", v1, v2, v3);
//	printf("topology_hash: v1 is %s, v2 is %s, v is %s\n",v1,v2,v);
    for (h = 0; *v != '\0'; v++)
        h = (64 * h + *v) % M_f;
    return h;
}

// NOTE THAT new_quality() RETURNS A fact STRUCTURE ENCAPSULATING A quality STRUCTURE.
struct fact *new_quality(char *a1, char *prop1, char *val1, struct asset_hashtable *a_ht,
                         struct identifier_hashtable *i_ht) {
    //make the quality
    struct quality *qual = (struct quality *) malloc(sizeof(struct quality));
    // needs to short circuit out if asset does not exist
    qual->a = (insert_and_get_asset(a1, a_ht, i_ht))->a;
    qual->property = (insert_identifier(prop1, PROPERTY, i_ht))->i;
    qual->value = (insert_identifier(val1, VALUE, i_ht))->i;
    //make the fact
    struct fact *f = (struct fact *) malloc(sizeof(struct fact));
    f->q = qual;
    f->type = QUAL;
    return f;
}

// NOTE THAT new_topology() RETURNS A fact STRUCTURE ENCAPSULATING A topology STRUCTURE.
struct fact *new_topology(char *asset1, char *asset2, char *rel, struct asset_hashtable *a_ht,
                          struct identifier_hashtable *i_ht) {
    //make the topology
    struct topology *top = (struct topology *) malloc(sizeof(struct topology));
    // needs to short circuit out if assets do not exist
    top->a1 = (insert_and_get_asset(asset1, a_ht, i_ht))->a;
    top->a2 = (insert_and_get_asset(asset2, a_ht, i_ht))->a;
    top->relationship = (insert_identifier(rel, RELATIONSHIP, i_ht))->i;
    //make the fact
    struct fact *f = (struct fact *) malloc(sizeof(struct fact));
    f->type = TOPO;
    f->t = top;
    return f;
}

// make an empty fact hashtable, return it AND assign it to fact_ht
struct fact_hashtable *make_fact_hashtable(void) {
//	printf("Inside make_fact_hashtable\n");
    struct fact_hashtable *f_ht = (struct fact_hashtable *) malloc(sizeof(struct fact_hashtable));
    f_ht->size = M_f;
    f_ht->head = (struct fact_hashtable_node **) calloc(M_f, M_f * sizeof(struct fact_hashtable_node *));
    fact_ht = f_ht;
//	printf("f_ht is %p, fact_ht is %p\n",(void *) f_ht, (void *) fact_ht);
    return f_ht;
}

//find_quality() RETURNS A fact_hashtable_node POINTER TO THE QUALITY IT FINDS MATCHING THE ASSET NAME AND PROPERTY
// OR, FAILING THAT, IT RETURNS NULL
// THIS VERSION COMPUTES THE HASH
struct fact_hashtable_node *find_quality(char *a, char *prop, struct fact_hashtable *f_ht) {
    struct fact_hashtable_node *current_node;
    int done = 0;
    unsigned int index = quality_hash(a, prop);
    current_node = f_ht->head[index];
    while (!done) {
        if (current_node == NULL) {
            done = 1;
            break;
        }
        if (!strcmp(a, current_node->f->q->a->name->key) && !strcmp(prop, current_node->f->q->property->key)) {
            done = 1;
            break;
        }
        else current_node = current_node->next;
    }
    return current_node;
}

//find_topology() RETURNS A fact_hashtable_node POINTER TO THE TOPOLOGY IT FINDS MATCHING THE ASSET NAMES AND RELATIONSHIP
// OR, FAILING THAT, IT RETURNS NULL
//THIS VERSION COMPUTES THE HASH
struct fact_hashtable_node *find_topology(char *a1, char *a2, char *rel, struct fact_hashtable *f_ht) {
    struct fact_hashtable_node *current_node;
    int done = 0;
    unsigned int index = topology_hash(a1, a2, rel);
    current_node = f_ht->head[index];
    while (!done) {
        if (current_node == NULL) {
            done = 1;
            break;
        }
        if (!strcmp(a1, current_node->f->t->a1->name->key) && !strcmp(a2, current_node->f->t->a2->name->key) &&
            !strcmp(rel, current_node->f->t->relationship->key)) {
            done = 1;
            break;
        }
        else current_node = current_node->next;
    }
    return current_node;
}

//find_quality_with_hash() RETURNS A fact_hashtable_node POINTER TO THE QUALITY IT FINDS MATCHING THE ASSET NAME AND PROPERTY
// OR, FAILING THAT, IT RETURNS NULL
//THIS VERSION ACCEPTS A HASH VALUE AS AN ARGUMENT - IE IT DOES NOT COMPUTE THE HASH
struct fact_hashtable_node *find_quality_with_hash(char *a, char *prop, struct fact_hashtable *f_ht,
                                                   unsigned int index) {
    struct fact_hashtable_node *current_node;
    int done = 0;
//	printf("a %s prop %s index %d fact_ht %p\n",a,prop,index,(void *) f_ht);
    // unsigned int index = quality_hash(a,prop);
    current_node = f_ht->head[index];
    while (!done) {
//		printf("pts: %p\n",(void *) current_node);
        if (current_node == NULL) {/*printf("current_node qual is NULL\n");*/ done = 1;
            break;
        }
        if (!strcmp(a, current_node->f->q->a->name->key) && !strcmp(prop, current_node->f->q->property->key)) {
            done = 1;
            break;
        }
        else current_node = current_node->next;
    }
    return current_node;
}

//find_topology() RETURNS A fact_hashtable_node POINTER TO THE TOPOLOGY IT FINDS MATCHING THE ASSET NAMES AND RELATIONSHIP
// OR, FAILING THAT, IT RETURNS NULL
//THIS VERSION ACCEPTS A HASH VALUE AS AN ARGUMENT - IE IT DOES NOT COMPUTE THE HASH
struct fact_hashtable_node *find_topology_with_hash(char *a1, char *a2, char *rel, struct fact_hashtable *f_ht,
                                                    unsigned int index) {
    struct fact_hashtable_node *current_node;
    int done = 0;
    // unsigned int index = topology_hash(a1,a2,rel);
    current_node = f_ht->head[index];
    while (!done) {
        if (current_node == NULL) {
            done = 1;
            break;
        }
        if (!strcmp(a1, current_node->f->t->a1->name->key) && !strcmp(a2, current_node->f->t->a2->name->key) &&
            !strcmp(rel, current_node->f->t->relationship->key)) {
            done = 1;
            break;
        }
        else current_node = current_node->next;
    }
    return current_node;
}

struct fact_hashtable *insert_fact(struct fact *f, struct fact_hashtable *f_ht, struct identifier_hashtable *i_ht) {
    unsigned int index = fact_hash(f);
//	printf("Inside insert_fact\n");
//	printf("f_ht is %p; f is %p\n",(void *) f_ht, (void *) f);
    if (f == NULL) return f_ht;
    else {
        struct fact_hashtable_node *f_node = (struct fact_hashtable_node *) malloc(sizeof(struct fact_hashtable_node));
        f_node->f = f;
        f_node->next = f_ht->head[index];
        f_ht->head[index] = f_node;
        return f_ht;
    }
}

// inserts a quality into the fact hashtable and returns the corresponding fact hashtable node pointer
// if a quality already exists it returns the corresponding fact hashtable node pointer
struct fact *prepare_quality(char *a, char *prop, char *val, struct asset_hashtable *a_ht, struct fact_hashtable *f_ht,
                             struct identifier_hashtable *i_ht) {
    struct fact *f = NULL;
    struct fact_hashtable_node *f_node = NULL;
    unsigned int index = quality_hash(a, prop);
    // see if a quality already exists
    // if a quality does not exist, create it and return it as a fact,
    // else it already exists and we return NULL to signal nothing to add.
//	printf("before find_quality_with_hash; f_ht is %p\n", (void *) f_ht);
    if ((f_node = find_quality_with_hash(a, prop, f_ht, index)) == NULL) {
        //make and populate the quality structure
        f = new_quality(a, prop, val, a_ht, i_ht);
    }
//	printf("after find_quality_with_hash\n");
    return f;
}

// inserts a topology into the fact hashtable and returns the corresponding fact hashtable node pointer
// if a topology already exists it returns the corresponding fact hashtable node pointer
struct fact *prepare_topology(char *a1, char *a2, char *rel, struct asset_hashtable *a_ht, struct fact_hashtable *f_ht,
                              struct identifier_hashtable *i_ht) {
    struct fact *f = NULL;
    struct fact_hashtable_node *f_node = NULL;
    unsigned int index = topology_hash(a1, a2, rel);
//	printf("Inside prepare topology\n");
    // see if a topology already exists
    // if a topology does not exist, create it and return it as a fact,
    // else it already exists, return NULL to signal nothing to add.
    if ((f_node = find_topology_with_hash(a1, a2, rel, f_ht, index)) == NULL) {
        //make and populate the topology structure
//		printf("f_node is NULL so make a new topology\n");
        f = new_topology(a1, a2, rel, a_ht, i_ht);
    }
//	printf("near the end of prepare_topology and f is %p\n", (void *) f);
    return f;
}

// make a new network_state comprised of an asset hashtable and a fact hashtable
struct network_state *new_network_state(struct network_state *ns, struct asset_hashtable *a_ht,
                                        struct fact_hashtable *f_ht) {
    ns = (struct network_state *) malloc(sizeof(struct network_state));
    ns->a_ht = a_ht;
    ns->f_ht = f_ht;
    printf("New network state; s - %p; a - %p; f - %p\n", (void *) ns, (void *) ns->a_ht, (void *) ns->f_ht);
    return ns;
}

char *network_state_to_string(struct network_state *ns) {
    char *str = strdup("Network State: 0\n");
    asprintf(&str, "%s", asset_hashtable_to_string(ns->a_ht));
    asprintf(&str, "%s%s", str, fact_hashtable_to_string(ns->f_ht));
    return str;
}

char *fact_hashtable_to_string(struct fact_hashtable *f_ht) {
    int i;
    char tmp_str[30];
    char *str = "";
    struct fact_hashtable_node *current_pointer;
    asprintf(&str, "Fact Hashtable: %p - head: %p - size:%d\n", &f_ht, f_ht->head, f_ht->size);
    for (i = 0; i < f_ht->size; i++) {
//		asprintf(&str,"%s[%d] = %p\n",str,i,(void *) f_ht->head[i]);
        current_pointer = f_ht->head[i];
        while (current_pointer != NULL) {
            if (current_pointer->f->type == QUAL)
                asprintf(&str, "%s--Q: Asset-%s Property-%s Value-%s\n", str, current_pointer->f->q->a->name->key,
                         current_pointer->f->q->property->key, current_pointer->f->q->value->key);
            else
                asprintf(&str, "%s--T: A1-%s A2-%s Relationship-%s\n", str, current_pointer->f->t->a1->name->key,
                         current_pointer->f->t->a2->name->key, current_pointer->f->t->relationship->key);
            current_pointer = current_pointer->next;
        }
    }
    return str;
}

char *fact_to_dot(struct fact_hashtable *f_ht, struct identifier_hashtable *i_ht) {
    int i, j;
    char tmp_str[30];
    char *str = "";
    struct fact_hashtable_node *current_f_pointer;
    struct identifier_hashtable_node *current_id_pointer;
    asprintf(&str, "digraph facts {\n");
    for (i = 0; i < i_ht->size; i++) {
        current_id_pointer = i_ht->head[i];
        while (current_id_pointer != NULL) {
            if (current_id_pointer->i->type == PROPERTY)
                asprintf(&str, "%s%s [shape=box]\n", str, current_id_pointer->i->key);
            else {
                if (current_id_pointer->i->type == VALUE)
                    asprintf(&str, "%s%s [shape=octagon]\n", str, current_id_pointer->i->key);
                else if (current_id_pointer->i->type == ASSET)
                    asprintf(&str, "%s%s\n", str, current_id_pointer->i->key);
            }
            current_id_pointer = current_id_pointer->next;
        }
    }
    for (j = 0; j < f_ht->size; j++) {
        current_f_pointer = f_ht->head[j];
        while (current_f_pointer != NULL) {
            if (current_f_pointer->f->type == QUAL)
                asprintf(&str, "%s%s -> %s -> %s;\n", str, current_f_pointer->f->q->a->name->key,
                         current_f_pointer->f->q->property->key, current_f_pointer->f->q->value->key);
            else
                asprintf(&str, "%s%s -> %s [Label = \"%s\"];\n", str, current_f_pointer->f->t->a1->name->key,
                         current_f_pointer->f->t->a2->name->key, current_f_pointer->f->t->relationship->key);
            current_f_pointer = current_f_pointer->next;
        }
    }
    asprintf(&str, "%s\n}\n", str);
    return str;
}

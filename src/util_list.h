/** \file util_list.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date July 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 *
 * Doubly Linked List
 * It is the programmers responsibility to free the value
 * Freeing the list does not free whatever its Nodes point to
 */

#ifndef UTIL_LIST_H
#define UTIL_LIST_H

#include <stdio.h>

struct Node {
	struct Node *prev;
	struct Node *next;
	void *val;
};

struct List {
	int size;
	struct Node *head;
	struct Node *tail;
};

struct ListArray {
	int size;
	void **val;
};

struct ListIterator {
	struct List *list;
	int idx;
};

#define list_print(LIST, TYPE, fmt) {		\
	struct Node *curr = LIST->head;		\
	while(curr) {				\
		printf(fmt, (TYPE)curr->val);	\
		curr = curr->next; }		\
	printf("\n"); }

struct List *list_new(void);
void list_free(struct List *);

int list_push(struct List *, void *val);
int list_rpush(struct List *, void *val);

void *list_pop(struct List *);
void *list_rpop(struct List *);
void *list_del(struct List *, int idx);

void *list_at(struct List *, int idx);
int list_size(struct List *);
int list_empty(struct List *);

void list_iterate(struct List *, void (callback)(void *));
void list_aggregate(struct List *, struct List *);

struct ListArray *list_to_array(struct List *);
void listarray_free(struct ListArray *);

#endif //UTIL_LIST_H

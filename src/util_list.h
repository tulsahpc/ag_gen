/** \file util_list.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date July 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.

   Doubly Linked List
   It is the programmers responsibility to free the value.
   Freeing the list does not free whatever its Nodes point
   to.
 */

#ifndef C_UTIL_LIST_H
#define C_UTIL_LIST_H

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

#define ListPrint(LIST, TYPE, fmt) {		\
	struct Node *curr = LIST->head;		\
	while(curr) {				\
		printf(fmt, (TYPE)curr->val);	\
		curr = curr->next; }		\
	printf("\n"); }

struct List *ListNew(void);
void ListFree(struct List *);

int ListAppend(struct List *, void *val);
int ListPrepend(struct List *, void *val);

void *ListPop(struct List *);
void *ListRemove(struct List *, int idx);

void *ListGet(struct List *, int idx);
int ListSize(struct List *);

struct ListArray *ListAsArray(struct List *);
void ListArrayFree(struct ListArray *);

#endif //C_UTIL_LIST_H

/** \file util_list.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date July 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "util_list.h"

static void list_iter_init(struct ListIterator *it, struct List *lst)
{
	it->list = lst;
	it->idx = 0;
}

static void *list_iter(struct ListIterator *it)
{
	void *val;
	if((val = list_at(it->list, it->idx)) != NULL) {
		it->idx++;
		return val;
	} else {
		return NULL;
	}
}

struct List *list_new(void)
{
	struct List *list = calloc(1, sizeof(struct List));
	return list;
}

void list_free(struct List *list)
{
	struct Node *curr = list->head;
	while(curr != NULL) {
		struct Node *tmp = curr;
		curr = curr->next;
		free(tmp);
	}
	free(list);
}

int list_push(struct List *list, void *val)
{
	struct Node *node = calloc(1, sizeof(struct Node));
	if(!node) return -1;
	node->val = val;

	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		struct Node *last_node = list->tail;

		last_node->next = node;
		node->prev = last_node;
		list->tail = node;
	}

	list->size++;
	return 0;
}

int list_rpush(struct List *list, void *val)
{
	struct Node *node = calloc(1, sizeof(struct Node));
	if(!node) return -1;
	node->val = val;

	if(!(list->head)) {
		list->head = node;
		list->tail = node;
	} else {
		struct Node *first_node = list->head;

		list->head = node;
		node->prev = NULL;
		node->next = first_node;

		first_node->prev = node;
	}

	list->size++;
	return 0;
}

void *list_pop(struct List *list)
{
	return list_del(list, list->size-1);
}

void *list_del(struct List *list, int idx)
{
	struct Node *curr = list->head;
	int i = 0;

	// Traverse the list until the requested index is reached.
	while(curr && i < idx) {
		curr = curr->next;
		i++;
	}

	// Return if the requested index is larger than the size
	// or if the current node is NULL.
	if(i < idx)
		return NULL;

	if(!curr)
		return NULL;

	// Get links to the appropriate nodes to switch them around
	struct Node *prevNode = curr->prev;
	struct Node *nextNode = curr->next;

	if(prevNode)
		prevNode->next = nextNode;
	if(nextNode)
		nextNode->prev = prevNode;

	// head/tail should only be NULL if the list is empty
	if(list->head == curr && nextNode == NULL) {
		list->head = NULL;
	}

	if(list->tail == curr && prevNode == NULL) {
		list->tail = NULL;
	}

	list->size--;

	// Now remove the node
	void *val = curr->val;
	free(curr);

	return val;
}

void *list_at(struct List *list, int idx)
{
	struct Node *curr = list->head;
	int i = 0;

	while(curr && i < idx) {
		curr = curr->next;
		i++;
	}

	if(!curr) return NULL;

	void *val = curr->val;
	return val;
}

int list_size(struct List *list)
{
	return list->size;
}

int list_empty(struct List *list)
{
	if(list->size == 0) return true;
	else return false;
}

struct ListIterator *list_new_iterator(struct List *lst)
{
	struct ListIterator *lstit = malloc(sizeof(struct ListIterator));
	list_iter_init(lstit, lst);
	return lstit;
}

void list_iterate(struct List *list, void (*callback)(void *))
{
	struct ListIterator it;
	list_iter_init(&it, list);
	void *val;
	while((val = list_iter(&it)) != NULL) {
		callback(val);
	}
}

struct ListArray *list_to_array(struct List *list)
{
	struct Node *cur = list->head;
	int len = list->size;
	void **arr = malloc(len * sizeof(void *));

	for(int i=0; i<len; i++) {
		arr[i] = cur->val;
		cur = cur->next;
	}

	struct ListArray *list_array = malloc(sizeof(struct ListArray));
	list_array->size = len;
	list_array->val = arr;

	return list_array;
}

void listarray_free(struct ListArray *list)
{
	free(list->val);
	free(list);
}

void _ListPrint(struct List *list)
{
	struct Node *curr = list->head;
	while(curr) {
		printf("%d\n", (int)curr->val);
		curr = curr->next;
	}
}

/** \file util_list.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date July 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <stdio.h>
#include "util_list.h"

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
	if(!node) return 1;
	node->val = val;

	if(!(list->head)) {
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
	if(!node) return 1;
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

	while(curr && i < idx) {
		curr = curr->next;
		i++;
	}

	if(!curr)
		return NULL;

	struct Node *prevNode = curr->prev;
	struct Node *nextNode = curr->next;

	if(prevNode)
		prevNode->next = nextNode;
	if(nextNode)
		nextNode->prev = prevNode;

	void *val = curr->val;
	free(curr);

	list->size--;

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

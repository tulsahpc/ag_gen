#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>

struct list {
    struct node *head;
    struct node *tail;
    size_t size;
};

struct node {
    void* data;
    struct node *prev;
    struct node *next;
};

struct list *list_new();
void list_add(struct list *l, void* ptr);
void *list_get_idx(struct list *l, size_t idx);
void list_rem_idx(struct list *l, size_t idx);

#endif

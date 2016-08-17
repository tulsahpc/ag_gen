#ifndef UTIL_DYNARRAY_H
#define UTIL_DYNARRAY_H

#include <stdlib.h>

struct DynArray {
	int size;
	int filled;
	void **arr;
};

struct DynArray *dynarray_new(void);
void dynarray_free(struct DynArray *);

int dynarray_insert(struct DynArray *, int, void *);
int dynarray_append(struct DynArray *, void *);
void *dynarray_remove(struct DynArray *, int);
void *dynarray_get(struct DynArray *, int);

#endif //UTIL_DYNARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util_dynarray.h"

#define DEFAULT_ARRAY_SIZE 10
#define GROWTH_FACTOR 2

static void *expand_array(struct DynArray *arr)
{
	arr->size *= GROWTH_FACTOR * sizeof(void *);
	void **new_ptr = realloc(arr, arr->size);
	if(new_ptr == NULL) {
		fprintf(stderr, "Error reallocating memory.");
		goto fatal;
	}
	return new_ptr;
fatal:
	free(new_ptr);
	return NULL;
}

static void *expand_array_to(struct DynArray *arr, int size)
{
	while(arr->size < size)
		arr->size *= GROWTH_FACTOR * sizeof(void *);

	void **new_ptr = realloc(arr, arr->size);
	if(new_ptr == NULL) {
		fprintf(stderr, "Error reallocating memory.");
		goto fatal;
	}

	return new_ptr;
fatal:
	free(new_ptr);
	return NULL;
}

struct DynArray *dynarray_new()
{
	struct DynArray *dyn = malloc(sizeof(struct DynArray));
	dyn->size = DEFAULT_ARRAY_SIZE;
	dyn->arr = calloc(1, sizeof(void *) * DEFAULT_ARRAY_SIZE);
	return dyn;
}

void dynarray_free(struct DynArray *arr)
{
	free(arr->arr);
	free(arr);
}

int dynarray_insert(struct DynArray *arr, int idx, void *val)
{
	if(arr->size < idx) {
		return EXIT_FAILURE;
	}

	arr->arr[idx] = val;
	return EXIT_SUCCESS;
}

int dynarray_append(struct DynArray *arr, void *val)
{

	return EXIT_SUCCESS;
}

void *dynarray_remove(struct DynArray *arr, int idx)
{

}

void *dynarray_get(struct DynArray *arr, int idx)
{

}

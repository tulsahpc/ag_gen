//
// Created by Kyle Cook on 8/2/17.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util/common.h"
#include "util/str_array.h"

str_array* new_str_array(void) {
    str_array* arr = (str_array*) getmem(sizeof(str_array));
    init_str_array(arr);
    return arr;
}

void init_str_array(str_array* arr) {
    arr->arr = (char**) getmem(INIT_SIZE * sizeof(char*));
    arr->size = INIT_SIZE;
    arr->used = 0;
}

void add_str(str_array* arr, char* str) {
    if(arr->size == arr->used) {
        arr->arr = (char**) realloc(arr->arr, arr->size*2*sizeof(char*));
        arr->size *= 2;
    }
    arr->arr[arr->used++] = dynstr(str);
}

void free_str_array(str_array* arr) {
	for(int i=0; i<arr->used; i++) {
		free(arr->arr[i]);
	}
	free(arr->arr);
}

void print_str_array(str_array* arr) {
	for(int i=0; i<arr->used; i++) {
		printf("%s\n", arr->arr[i]);
	}
}

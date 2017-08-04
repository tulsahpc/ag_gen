//
// Created by Kyle Cook on 8/2/17.
//

#include <string.h>
#include <stdlib.h>

#include "str_util.h"

void* getmem(size_t size) {
	void* data = malloc(size);
	if(data == NULL)
		exit(EXIT_FAILURE);
	return data;
}

char* getstr(size_t size) {
	char* mystring = (char*) getmem(size+1);
	mystring[size] = '\0';
	return mystring;
}

char* dynstr(const char* str) {
	size_t len = strlen(str);
	char* mystring = (char*) getstr(len);
	memcpy(mystring, str, len);
	return mystring;
}

const int INIT_SIZE = 50;
void init_str_array(str_array* arr) {
    arr->arr = (char**) getmem(INIT_SIZE);
    arr->size = INIT_SIZE;
    arr->used = 0;
}

void add_str(str_array* arr, char* str) {
    if(arr->size == arr->used) {
        arr->arr = (char**) realloc(arr->arr, arr->size*2*sizeof(char*));
        arr->size *= 2;
    }
    arr->arr[arr->used++] = str;
}

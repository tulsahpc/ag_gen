#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/mem.h"

// Always free the string when no longer required
char *dynstr(const char *str) {
	size_t str_len = strlen(str);
	char *new_str = (char *) malloc(sizeof(char) * (str_len + 1));
	if (new_str == NULL)
		return (char *) -1;

	strncpy(new_str, str, str_len);
	return new_str;
}

void* getmem(size_t size) {
	void* data = calloc(1, size);
	if(data == NULL) {
		fprintf(stderr, "Could not allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	return data;
}

void clearmem(void* data, size_t size) {
	memset(data, 0, size);
}

char* getstr(size_t size) {
	char* mystring = (char*) getmem(size+1);
	mystring[size] = '\0';
	return mystring;
}

//
// Created by Kyle Cook on 8/2/17.
//

#include <string.h>

void* getmem(size_t size) {
	void* data = malloc(size);
	if(data == NULL)
		fatal("No memory available.");
	return data;
}

char* getstr(size_t size) {
	char* mystring = (char*) getmem(size+1);
	mystring[size] = '\0';
	return mystring;
}

char* dynstr(char* str) {
	size_t len = strlen(str);
	char* mystring = (char*) getstr(len);
	memcpy(mystring, str, len);
}

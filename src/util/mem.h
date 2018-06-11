#ifndef _MEM_H
#define _MEM_H

#include <stddef.h>

char *dynstr(const char *str);
void *getmem(size_t size);
void *getcmem(size_t size);
void clearmem(void *data, size_t size);
char *getstr(size_t size);
void itoa(int n, char s[]);
void reverse(char s[]);

#endif //_MEM_H

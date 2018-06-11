#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/mem.h"

#ifdef UNIT_TESTING
extern void* _test_malloc(const size_t size, const char* file, const int line);
extern void* _test_calloc(const size_t number_of_elements, const size_t size,
                          const char* file, const int line);
extern void _test_free(void* const ptr, const char* file, const int line);

#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
#endif // UNIT_TESTING

void *getmem(size_t size) {
    void *buf = malloc(size);
    if(buf == NULL) {
        fprintf(stderr, "Could not allocate memory.\n");
        exit(1);
    }
    return buf;
}

void *getcmem(size_t size) {
    void *buf = calloc(1, size);
    if (buf == NULL) {
        fprintf(stderr, "Could not allocate memory.\n");
        exit(1);
    }
    return buf;
}

void clearmem(void *buf, size_t size) {
    memset(buf, 0, size);
}

char *getstr(size_t size) {
    char *mystring = (char *)getmem(size + 1);
    mystring[size] = '\0';
    return mystring;
}

char *dynstr(const char *str) {
    size_t str_len = strlen(str);
    char *new_str = getstr(str_len);
    if (new_str == NULL)
        return (char *)-1;

    strncpy(new_str, str, str_len);
    return new_str;
}

// Shamelessly stolen from http://clc-wiki.net/wiki/K%26R2_solutions:Chapter_3:Exercise_4
void itoa(int n, char s[]) {
    int i, sign;
    sign = n;

    i = 0;
    do {
        s[i++] = abs(n % 10) + '0';
    } while ( n /= 10 );
    if (sign < 0)
        s[i++] = '-';

    s[i] = '\0';
    reverse(s);
}

// http://clc-wiki.net/wiki/K%26R2_solutions:Chapter_3:Exercise_4
void reverse(char s[]) {
    int c, i, j;
    for (i=0, j=strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

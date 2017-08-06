//
// Created by Kyle Cook on 8/2/17.
//

#ifndef AG_GEN_STR_UTIL_H
#define AG_GEN_STR_UTIL_H

#include <string.h>

struct str_array {
    char** arr;
    size_t size;
    size_t used;
};

typedef struct str_array str_array;

void* getmem(size_t size);
char* getstr(size_t size);
char* dynstr(const char* str);

str_array* new_str_array(void);
void init_str_array(str_array* arr);
void add_str(str_array* arr, char* str);
void free_str_array(str_array* arr);
void print_str_array(str_array* arr);

#endif //AG_GEN_STR_UTIL_H

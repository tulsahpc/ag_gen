//
// Created by Kyle Cook on 8/2/17.
//

#ifndef AG_GEN_STR_UTIL_H
#define AG_GEN_STR_UTIL_H

#include <string.h>

#define INIT_SIZE 20

struct str_array {
    char **arr;
    size_t size;
    size_t used;
};

typedef struct str_array str_array;

str_array *new_str_array(void);
void init_str_array(str_array *arr);
void add_str(str_array *arr, char *str);
char *get_str_idx(str_array *arr, int idx);
void free_str_array(str_array *arr);
void print_str_array(str_array *arr);

#endif // AG_GEN_STR_UTIL_H

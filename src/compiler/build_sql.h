//
// Created by Kyle Cook on 8/3/17.
//

#ifndef AG_GEN_BUILD_SQL_H
#define AG_GEN_BUILD_SQL_H

typedef struct {
	char** elts;
	int count;
} str_list;

void init_list(str_list* l);
void add_entry(str_list* l, char* name);
void free_list(str_list* l);
void print_list(str_list* l);

#endif //AG_GEN_BUILD_SQL_H

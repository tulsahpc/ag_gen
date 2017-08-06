//
// Created by Kyle Cook on 8/3/17.
//

#include <stdio.h>
#include <stdlib.h>
#include "str_util.h"
#include "build_sql.h"

void init_list(str_list* l) {
	l->elts = (char**) getmem(100);
	l->count = 0;
}

void add_entry(str_list* l, char* name) {
	char* assetname = getstr(100);
	snprintf(assetname, 100, "(DEFAULT, '%s', (SELECT id FROM network WHERE name = 'home')),", name);
	l->elts[l->count++] = assetname;
}

void free_list(str_list* l) {
	for(int i=0; i<l->count; i++) {
		free(l->elts[i]);
	}
	free(l->elts);
}

void print_list(str_list* l) {
	if(l->count == 0)
		return;
    for(int i=0; i<l->count; i++) {
        printf("%s\n", l->elts[i]);
    }
}

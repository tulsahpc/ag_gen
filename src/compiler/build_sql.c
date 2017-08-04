//
// Created by Kyle Cook on 8/3/17.
//

#include <stdio.h>
#include <stdlib.h>
#include "str_util.h"
#include "build_sql.h"

int assetcount = 0;

void init_asset_list(void) {
	assets = (char**)malloc(sizeof(char**) * 100);
}

void new_asset(char* name) {
	char* assetname = getstr(100);
	snprintf(assetname, 100, "(DEFAULT, '%s', (SELECT id FROM network WHERE name = 'home')),", name);
	assets[assetcount++] = assetname;
}

void free_asset_list(void) {
	for(int i=0; i<assetcount; i++) {
		free(assets[i]);
	}
	free(assets);
}

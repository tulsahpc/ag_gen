#ifndef _BUILD_SQL_H
#define _BUILD_SQL_H

struct statement {
    char* obj;
    char* op;
    char* val;
};

int assetcount;

char* make_asset(char* as);
char* make_quality(int assetid, struct statement* st);
char* make_topology(int fromasset, int toasset, char* options);

#endif //_BUILD_SQL_H

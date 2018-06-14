#ifndef _BUILD_SQL_H
#define _BUILD_SQL_H

#include "util/list.h"
#include "util/str_array.h"

#define GLOBAL_SIZE 4

struct exploitpattern {
    char* name;
    str_array* params;
    str_array* preconditions;
    struct list* postconditions;
};

struct postcondition {
    char *op;
    char *fact;
};

struct statement {
    char *obj;
    char *op;
    char *val;
};

char *make_asset(char *as);
char *make_quality(int assetid, struct statement *st);
char *make_topology(int fromasset, int toasset, char *dir,
                    struct statement *st);

char *make_exploit(struct exploitpattern *xp);
char *make_precondition();
char *make_postcondition();

#endif //_BUILD_SQL_H

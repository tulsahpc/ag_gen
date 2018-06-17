#ifndef _BUILD_SQL_H
#define _BUILD_SQL_H

#include "util/list.h"
#include "util/str_array.h"
#include "util/hash.h"

typedef enum FACT_T {
    QUALITY_T,
    TOPOLOGY_T,
} FACT_T;

typedef struct exploit_instance {
    int id;
    char *sql;
} exploit_instance;

typedef struct exploitpattern {
    char* name;
    str_array* params;
    struct list* preconditions;
    struct list* postconditions;
} exploitpattern;

typedef struct statement {
    char *obj;
    char *op;
    char *val;
} statement;

typedef struct fact {
    FACT_T type;
    char *from;
    char *dir;
    char *to;
    statement* st;
} fact;

typedef struct postcondition {
    char *op;
    fact *fact;
} postcondition;

char *make_asset(char *as);
char *make_quality(int assetid, statement *st);
char *make_topology(int fromasset, int toasset, char *dir,
                    statement *st);

void print_fact(fact *fct);

exploit_instance *make_exploit(exploitpattern *xp);
char *make_precondition(hashtable *exploit_ids, exploitpattern *xp, fact *fct);
char *make_postcondition(exploitpattern *xp);

#endif //_BUILD_SQL_H

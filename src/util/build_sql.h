#ifndef _BUILD_SQL_H
#define _BUILD_SQL_H

struct statement {
    char *obj;
    char *op;
    char *val;
};

char *make_asset(char *as);
char *make_quality(int assetid, struct statement *st);
char *make_topology(int fromasset, int toasset, char *dir,
                    struct statement *st);

char *make_exploit();
char *make_precondition();
char *make_postcondition();

#endif //_BUILD_SQL_H

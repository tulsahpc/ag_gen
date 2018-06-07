#include <stdio.h>
#include <string.h>

#include "util/build_sql.h"
#include "util/mem.h"
#include "util/list.h"

int assetcount = 0;
int exploitcount = 0;

const char *sqlAsset =
    "(%d, '%s', (SELECT id FROM network WHERE name = 'home')),";
const char *sqlQuality = "(%d, '%s', '%s', '%s'),";
const char *sqlTopology = "(%d, %d, '%s', '%s', '%s', '%s'),";
const char *sqlExploit = "(%d, '%s', %d)";

char *make_asset(char *as) {
    size_t mystringlen = strlen(sqlAsset) + strlen(as);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlAsset, assetcount++, as);
    return mystring;
}

char *make_quality(int assetid, struct statement *st) {
    size_t mystringlen =
        32 + strlen(st->obj) + strlen(st->op) + strlen(st->val);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlQuality, assetid, st->obj, st->op, st->val);
    return mystring;
}

char *make_topology(int fromasset, int toasset, char *dir,
                    struct statement *st) {
    char *prop = st->obj;
    char *op = st->op;
    char *val = st->val;

    size_t mystringlen =
        32 + 32 + strlen(dir) + strlen(prop) + strlen(op) + strlen(val);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlTopology, fromasset, toasset, dir, prop, op, val);
    return mystring;
}

char *make_exploit(struct list *xplist) {
    for(int i=0; i<xplist->size; i++) {
        struct exploitpattern *xp = list_get_idx(xplist, i);

        if(xp->global == 1) {
        }

        if(xp->group != NULL) {
        }

        for(int j=0; j<xp->params->used; j++) {
        }

        if(xp->options != NULL) {
            for(int j=0; j<xp->options->used; j++) {
            }
        }

        for(int j=0; j<xp->preconditions->used; j++) {
        }

        for(int j=0; j<xp->postconditions->size; j++) {
            struct postcondition *pc = list_get_idx(xp->postconditions, j);
        }
    }
}

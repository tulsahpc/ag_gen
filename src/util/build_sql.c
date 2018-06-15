#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/build_sql.h"
#include "util/mem.h"
#include "util/list.h"
#include "util/vector.h"

#define BUFSPACE 64

int assetcount = 0;
int exploitcount = 0;

const char *sqlAsset =
    "(%d, '%s', (SELECT id FROM network WHERE name = 'home')),";
const char *sqlQuality = "(%d, '%s', '%s', '%s'),";
const char *sqlTopology = "(%d, %d, '%s', '%s', '%s', '%s'),";
const char *sqlExploit = "\t(%d, '%s', %d),\n";
const char *sqlPrecondition = "\t(%d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s')";

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

static int exploit_curr_id = 0;
char *make_exploit(struct exploitpattern *xp) {
    size_t len = strlen(sqlExploit) + strlen(xp->name) + BUFSPACE;
    char *buf = malloc(len);
    sprintf(buf, sqlExploit, exploit_curr_id++, xp->name, xp->params->used);
    return buf;
}

static int precondition_curr_id = 0;
char *make_precondition(struct exploitpattern *xp) {
    size_t bufsize = INITIALBUFSIZE;
    char *buf = malloc(bufsize);
    for(int i=0; i<xp->preconditions->size; i++) {
        struct statement *st = list_get_idx(xp->preconditions, i);
        char *st_str = strlen(sqlPrecondition) +
        sprintf(buf, sqlPrecondition, precondition_curr_id++, exploit_curr_id-1,
            );

        while(bufsize < strlen(buf) + strlen(sqladd))
            buf = realloc(buf, (bufsize*=2));
        strcat(buf, sqladd);
    }
    char *last = strrchr(buf, ',');
    *last = ';';
    printf("%s\n", buf);
}

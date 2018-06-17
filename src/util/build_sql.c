#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/build_sql.h"
#include "util/mem.h"
#include "util/list.h"
#include "util/vector.h"
#include "util/hash.h"

#define BUFSPACE 64

int assetcount = 0;
int exploitcount = 0;

const char *sqlAsset =
    "(%d, '%s', (SELECT id FROM network WHERE name = 'home')),";
const char *sqlQuality = "(%d, '%s', '%s', '%s'),";
const char *sqlTopology = "(%d, %d, '%s', '%s', '%s', '%s'),";
const char *sqlExploit = "\t(%d, '%s', %d),\n";
const char *sqlPrecondition = "\t(%d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s')\n";

char *make_asset(char *as) {
    size_t mystringlen = strlen(sqlAsset) + strlen(as);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlAsset, assetcount++, as);
    return mystring;
}

char *make_quality(int assetid, statement *st) {
    size_t mystringlen =
        32 + strlen(st->obj) + strlen(st->op) + strlen(st->val);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlQuality, assetid, st->obj, st->op, st->val);
    return mystring;
}

char *make_topology(int fromasset, int toasset, char *dir,
                    statement *st) {
    char *prop = st->obj;
    char *op = st->op;
    char *val = st->val;

    size_t mystringlen =
        32 + 32 + strlen(dir) + strlen(prop) + strlen(op) + strlen(val);
    char *mystring = getstr(mystringlen);
    sprintf(mystring, sqlTopology, fromasset, toasset, dir, prop, op, val);
    return mystring;
}

static int precondition_curr_id = 0;
char *make_precondition(hashtable *exploit_ids, exploitpattern *xp, fact *fct) {
    char *buf = malloc(1000);
    if(fct->type == QUALITY_T) {
        int from_id = (int)get_hashtable(exploit_ids, fct->from);
        char *obj = fct->st->obj;
        char *op = fct->st->op;
        char *val = fct->st->val;

        hashtable *params = str_array_to_hashtable(xp->params);

        printf("param: %s\n", fct->from);
        printf("idx: %d\n\n", get_hashtable(exploit_ids, fct->from));

        sprintf(buf, sqlPrecondition, precondition_curr_id, from_id, QUALITY_T, get_hashtable(params, fct->from), NULL, obj, val, op, NULL);

    } else {
        int from_id = (int)get_hashtable(exploit_ids, fct->from);
        int to_id = (int)get_hashtable(exploit_ids, fct->to);
    }

    return buf;
}

static int exploit_curr_id = 0;
exploit_instance *make_exploit(exploitpattern *xp) {
    size_t len = strlen(sqlExploit) + strlen(xp->name) + BUFSPACE;
    char *buf = malloc(len);
    sprintf(buf, sqlExploit, exploit_curr_id, xp->name, xp->params->used);
    exploit_instance *ei = getmem(sizeof(struct exploit_instance));
    ei->id = exploit_curr_id++;
    ei->sql = buf;
    return ei;
}

void print_fact(fact *fct) {
    char *type;
    if(fct->type == QUALITY_T) {
        printf("quality:%s,%s%s%s\n", fct->from, fct->st->obj, fct->st->op, fct->st->val);
    }
    else {
        printf("topology:%s%s%s,%s%s%s\n", fct->from, fct->dir, fct->to, fct->st->obj, fct->st->op, fct->st->val);
    }
}

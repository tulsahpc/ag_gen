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
    "(%d, '%s'),";
const char *sqlQuality = "(%d, '%s', '%s', '%s'),";
const char *sqlTopology = "(%d, %d, '%s', '%s', '%s', '%s'),";
const char *sqlExploit = "\t(%d, '%s', %d),\n";
const char *sqlPrecondition = "\t(%d, %d, %d, %d, %d, '%s', '%s', '%s', '%s'),\n";
const char *sqlPostcondition = "\t(%d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s'),\n";

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

static int postcondition_curr_id = 0;
char *make_postcondition(hashtable *exploit_ids, exploitpattern *xp, postcondition *pc) {
    char *buf = malloc(1000);

    int exploit_id = (int)get_hashtable(exploit_ids, xp->name);
    hashtable *params = str_array_to_hashtable(xp->params);
    char *action = pc->op;
    ACTION_T action_type;

    if(strcmp(action, "add") == 0)
        action_type = ADD_T;
    else if(strcmp(action, "insert") == 0)
        action_type = ADD_T;
    else if(strcmp(action, "update") == 0)
        action_type = UPDATE_T;
    else if(strcmp(action, "delete") == 0)
        action_type = DELETE_T;
    else {
        printf("Unknown action '%s'. Aborting.\n", action);
        exit(1);
    }

    if(pc->fact->type == QUALITY_T) {
        char *obj = pc->fact->st->obj;
        char *op = pc->fact->st->op;
        char *val = pc->fact->st->val;

        sprintf(buf, sqlPostcondition, postcondition_curr_id++, exploit_id, QUALITY_T, get_hashtable(params, pc->fact->from), NULL, obj, val, op, NULL, action);

    } else {
        char *obj = pc->fact->st->obj;
        char *op = pc->fact->st->op;
        char *val = pc->fact->st->val;
        char *dir = pc->fact->dir;

        sprintf(buf, sqlPostcondition, postcondition_curr_id++, exploit_id, TOPOLOGY_T, get_hashtable(params, pc->fact->from), get_hashtable(params, pc->fact->to), obj, val, op, dir, action);
    }

    return buf;
}

static int precondition_curr_id = 0;
char *make_precondition(hashtable *exploit_ids, exploitpattern *xp, fact *fct) {
    char *buf = malloc(1000);

    int exploit_id = (int)get_hashtable(exploit_ids, xp->name);
    hashtable *params = str_array_to_hashtable(xp->params);

    if(fct->type == QUALITY_T) {
        char *obj = fct->st->obj;
        char *op = fct->st->op;
        char *val = fct->st->val;

        sprintf(buf, sqlPrecondition, precondition_curr_id++, exploit_id, QUALITY_T, get_hashtable(params, fct->from), NULL, obj, val, op, NULL);

    } else {
        char *obj = fct->st->obj;
        char *op = fct->st->op;
        char *val = fct->st->val;
        char *dir = fct->dir;

        sprintf(buf, sqlPrecondition, precondition_curr_id++, exploit_id, TOPOLOGY_T, get_hashtable(params, fct->from), get_hashtable(params, fct->to), obj, val, op, dir);
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

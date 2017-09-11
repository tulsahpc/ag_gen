%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "str_util.h"
    #include "util_hash.h"
    #include "build_sql.h"

    #define YYDEBUG 0

    struct networkmodel {
        str_array* assets;

        hashtable* asset_tab;
        int numassets;

        str_array* facts;
    };

    int yylex();
    void yyerror(struct networkmodel* nm, char const *s);
    extern FILE* yyin;
    extern int yylineno;

    extern int assetcount;
    extern int factcount;
%}

%union {
    struct str_array* arr;
    struct networkmodel* model;
    struct statement* st;
    char* string;
}

%parse-param { struct networkmodel* nm }

%type <arr> assetlist assets factlist facts
%type <string> relop operator direction number value asset fact
%type <st> statement

%token <string> IDENTIFIER INT FLOAT
%token <string> EQ GT LT GEQ LEQ
%token <string> ONEDIR BIDIR
%token NETWORK MODEL ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE;

%%

root: NETWORK IDENTIFIER EQ assets facts PERIOD {
    nm->assets = $4;
    nm->facts = $5;
  }
;

assets: ASSETS COLON assetlist { $$ = $3; }

assetlist: { $$ = NULL; }
| assetlist asset {
    if($1 == NULL) {
        add_hashtable(nm->asset_tab, $2, assetcount);
        $$ = new_str_array();
        char* sql = make_asset($2);
        add_str($$, sql);
    } else {
        add_hashtable(nm->asset_tab, $2, assetcount);
        char* sql = make_asset($2);
        add_str($1, sql);
        $$ = $1;
    }
  }

asset: IDENTIFIER SEMI { $$ = $1; }

facts: FACTS COLON factlist { $$ = $3; }

factlist: { $$ = NULL; }
| factlist fact {
    if($1 == NULL) {
        $$ = new_str_array();
        add_str($$, $2);
    } else {
        add_str($$, $2);
        $$ = $1;
    }
  }
;

fact:
  QUALITY COLON IDENTIFIER COMMA statement SEMI {
    int assetid = get_hashtable(nm->asset_tab, $3);
    char* sql = make_quality(assetid, $5);
    $$ = sql;
  }
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI {
    int fromasset = get_hashtable(nm->asset_tab, $3);
    int toasset = get_hashtable(nm->asset_tab, $5);

    struct statement* st = $7;

    char* options = getmem(128*sizeof(char));
    strncat(options, st->obj, strlen(st->obj));
    strncat(options, st->op, strlen(st->op));
    strncat(options, st->val, strlen(st->val));

    char* sql = make_topology(fromasset, toasset, options);
    $$ = sql;
  }
;

statement:
  IDENTIFIER {
    struct statement* st = getmem(sizeof(struct statement));
    st->obj = $1;
    st->op = "";
    st->val = "";
    $$ = st;
  }
| IDENTIFIER operator value {
    struct statement* st = getmem(sizeof(struct statement));
    st->obj = $1;
    st->op = $2;
    st->val = $3;
    $$ = st;
  }
;

value:
  IDENTIFIER
| number
;

number:
  INT
| FLOAT
;

operator:
  relop
| EQ { $$ = dynstr("="); }
;

relop:
  GT
| LT
| GEQ
| LEQ
;

direction:
  ONEDIR
| BIDIR
;

%%

int main(int argc, char** argv) {
    FILE* file;
    if(argv[1] == 0) {
        file = fopen("../examples/SystemV8.nm", "r");
    } else {
        file = fopen(argv[1], "r");
    }

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }

    struct networkmodel nm;
    nm.asset_tab = new_hashtable(101);

    //yydebug = 1;
    yyin = file;
    do {
        yyparse(&nm);
    } while(!feof(yyin));

    //printf("%s : %d\n", "flowmeter", get_hashtable(nm.asset_tab, "flowmeter"));
    print_str_array(nm.assets);
    print_str_array(nm.facts);

    free_hashtable(nm.asset_tab);
}

void yyerror(struct networkmodel* nm, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

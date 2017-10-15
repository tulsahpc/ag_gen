%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "str_util.h"
    #include "util_hash.h"
    #include "build_sql.h"

    #define YYDEBUG 0

    struct exploitpattern {
    };

    int yylex();
    void yyerror(struct exploitpattern* nm, char const *s);
    extern FILE* yyin;
    extern int yylineno;
%}

%union {
    struct str_array* arr;
    struct exploitpattern* model;
    struct statement* st;
    char* string;
}

%parse-param { struct exploitpattern* nm }

%type <arr> assetlist assets factlist facts
%type <string> relop operator direction number value asset fact
%type <st> statement

%token <string> IDENTIFIER INT FLOAT
%token <string> EQ GT LT GEQ LEQ
%token <string> ONEDIR BIDIR
%token NETWORK MODEL ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE;

%%

root: exploitlist exploit {

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
        file = fopen("../examples/SystemV8.xp", "r");
    } else {
        file = fopen(argv[1], "r");
    }

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }

    struct exploitmodel xm;
    xm.asset_tab = new_hashtable(101);

    //yydebug = 1;
    yyin = file;
    do {
        yyparse(&xm);
    } while(!feof(yyin));

    //printf("%s : %d\n", "flowmeter", get_hashtable(xm.asset_tab, "flowmeter"));
    //print_str_array(xm.assets);
    //print_str_array(xm.facts);

    str_array* qualities = new_str_array();
    str_array* topologies = new_str_array();

    for(int i=0; i<xm.facts->used; i++) {
        char* current = xm.facts->arr[i];
        char* copy = getstr(strlen(current));

        strncpy(copy, current, strlen(current));

        char* type = strsep(&copy, ":");
        if(strncmp(type, "q", 1) == 0) {
            add_str(qualities, copy);
        } else {
            add_str(topologies, copy);
        }
    }

    FILE* fp = fopen("test.sql", "w");
    if(fp == NULL) {
        printf("Error creating file.\n");
        exit(1);
    }

    char* assetheader = "INSERT INTO asset VALUES";
    fprintf(fp, "%s\n", assetheader);
    for(int i=0; i<xm.assets->used-1; i++) {
        char* nextstring = xm.assets->arr[i];
        fprintf(fp, "%s\n", nextstring);
    }
    char* stripped = xm.assets->arr[xm.assets->used-1];
    stripped[strlen(stripped)-1] = '\n';
    fprintf(fp, "%s\n", stripped);

    fprintf(fp, "%s\n", "ON CONFLICT DO NOTHING;");

    char* qualityheader = "\nINSERT INTO quality VALUES";
    fprintf(fp, "%s\n", qualityheader);
    for(int i=0; i<qualities->used-1; i++) {
        char* nextstring = qualities->arr[i];
        fprintf(fp, "%s\n", nextstring);
    }
    stripped = qualities->arr[xm.assets->used-1];
    stripped[strlen(stripped)-1] = '\n';
    fprintf(fp, "%s\n", stripped);

    fprintf(fp, "%s\n", "ON CONFLICT DO NOTHING;");

    char* topologyheader = "\nINSERT INTO topology VALUES";
    fprintf(fp, "%s\n", topologyheader);
    for(int i=0; i<topologies->used-1; i++) {
        char* nextstring = topologies->arr[i];
        fprintf(fp, "%s\n", nextstring);
    }
    stripped = topologies->arr[xm.assets->used-1];
    stripped[strlen(stripped)-1] = '\n';
    fprintf(fp, "%s\n", stripped);

    fprintf(fp, "%s\n", "ON CONFLICT DO NOTHING;");

    free_hashtable(xm.asset_tab);
}

void yyerror(struct exploitmodel* xm, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

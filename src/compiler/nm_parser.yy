%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "str_util.h"
    #include "util_hash.h"

    #define YYDEBUG 0

    struct networkmodel {
        str_array* assets;
        hashtable* asset_tab;
        int numassets;

        str_array* qualities;
        int numqualities;

        str_array* topologies;
        int numtopologies;
    };

    struct statement {
        char* obj;
        char* op;
        char* val;
    };

    const char* sqlAsset = "(%d, '%s', (SELECT id FROM network WHERE name = 'home')),";
    const char* sqlQuality = "(%d, '%s', '%s'),";

    int assetcount = 0;

    int yylex();
    void yyerror(struct networkmodel* nm, char const *s);
    extern FILE* yyin;
    extern int yylineno;

%}

%union {
    struct str_array* arr;
    struct networkmodel* model;
    char* string;
}

%parse-param { struct networkmodel* nm }

%type <arr> assetlist assets
%type <string> relop operator direction number value asset

%token <string> IDENTIFIER INT FLOAT
%token <string> EQ GT LT GEQ LEQ
%token <string> ONEDIR BIDIR
%token NETWORK MODEL ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE;

%%

root: NETWORK IDENTIFIER EQ assets facts PERIOD {
    nm->assets = $4;
  }
;

assets: ASSETS COLON assetlist { $$ = $3; }

assetlist: { $$ = NULL; }
| assetlist asset {
    if($1 == NULL) {
        $$ = new_str_array();
    } else {
        size_t mystringlen = strlen(sqlAsset) + strlen($2);
        char* mystring = getstr(mystringlen);
        add_hashtable(nm->asset_tab, $2, assetcount);
        sprintf(mystring, sqlAsset, assetcount++, $2);
        add_str($1, mystring);
        $$ = $1;
    }
  }

asset: IDENTIFIER SEMI { $$ = $1; }

facts: FACTS COLON factlist

factlist:
| factlist fact
;

fact:
  QUALITY COLON IDENTIFIER COMMA statement SEMI
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI
;

statement:
  IDENTIFIER
| IDENTIFIER operator value
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

    printf("%s : %d\n", "flowmeter", get_hashtable(nm.asset_tab, "flowmeter"));

    free_hashtable(nm.asset_tab);
}

void yyerror(struct networkmodel* nm, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

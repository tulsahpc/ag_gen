%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "str_util.h"
    #include "build_sql.h"

    #define YYDEBUG 0

    int yylex();
    void yyerror(char const *s);
    extern FILE* yyin;
    extern int yylineno;
%}

%union {
    struct str_array* arr;
    char* string;
}

%type <arr> assetlist
%type <string> relop operator direction number value statement

%token <string> IDENTIFIER INT FLOAT
%token <string> EQ GT LT GEQ LEQ
%token <string> ONEDIR BIDIR
%token NETWORK MODEL ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE;

%%

root: NETWORK IDENTIFIER EQ ASSETS COLON assetlist FACTS COLON factlist PERIOD
;

assetlist: { $$ = new_str_array(); }
| assetlist IDENTIFIER SEMI { $$ = $1; add_str($1, $2); printf("%d\n", ); }
;

factlist: 
| factlist fact
;

fact:
  QUALITY COLON IDENTIFIER COMMA statement SEMI
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI
;

statement:
  IDENTIFIER { $$ = $1; }
| IDENTIFIER operator value { $$ = $1; }
;

value:
  IDENTIFIER { $$ = $1; }
| number { $$ = $1; }
;

number:
  INT { $$ = $1; }
| FLOAT { $$ = $1; }
;

operator:
  relop { $$ = $1; }
| EQ { $$ = $1; }
;

relop:
  GT { $$ = $1; }
| LT { $$ = $1; }
| GEQ { $$ = $1; }
| LEQ { $$ = $1; }
;

direction:
  ONEDIR { $$ = $1; }
| BIDIR { $$ = $1; }
;

%%

int main(int argc, char** argv) {
    FILE* file;
    if(argv[1] == 0) {
        file = fopen("examples/SystemV8.nm", "r");
    } else {
        file = fopen(argv[1], "r");
    }

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }

    //yydebug = 1;
    yyin = file;
    do {
        yyparse();
    } while(!feof(yyin));
}

void yyerror(char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

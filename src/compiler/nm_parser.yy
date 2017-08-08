%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "str_util.h"

    #define YYDEBUG 0

    struct networkmodel {
        str_array* assets;
        str_array* facts;
    };

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

%type <arr> assetlist assets factlist facts
%type <string> relop operator direction number value statement asset fact

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
        $$ = new_str_array();
    } else {
        add_str($1, $2);
        $$ = $1;
    }
  }

asset: IDENTIFIER SEMI { $$ = $1; }

facts: FACTS COLON factlist { $$ = $3; }

factlist: { $$ = NULL; }
| factlist fact {
    if($1 == NULL) {
        $$ = new_str_array();
    } else {
        add_str($1, $2);
        $$ = $1;
    }
  }
;

fact:
  QUALITY COLON IDENTIFIER COMMA statement SEMI {
    int mystringlen = strlen($3) + strlen($5);
    char* mystring = getstr(mystringlen);
    sprintf(mystring, "%s%s", $3, $5);
    $$ = mystring;
  }
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI {

  }
;

statement:
  IDENTIFIER operator value {
    printf("%s\n", $1);
    printf("%s\n", $2);
    printf("%s\n\n", $3);
  }
| IDENTIFIER
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
| EQ
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
        file = fopen("examples/SystemV8.nm", "r");
    } else {
        file = fopen(argv[1], "r");
    }

    if(!file) {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }

    struct networkmodel nm;
    //yydebug = 1;
    yyin = file;
    do {
        yyparse(&nm);
    } while(!feof(yyin));

    //print_str_array(nm.assets);
    //print_str_array(nm.facts);
    free_str_array(nm.assets);
}

void yyerror(struct networkmodel* nm, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

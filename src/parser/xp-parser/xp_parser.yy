%debug
%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "util/mem.h"
    #include "util/str_array.h"
    #include "util/hash.h"
    #include "util/build_sql.h"

    #define YYDEBUG 1

    struct exploitpattern {
        str_array* options;
        str_array* preconditions;
        str_array* postconditions;
    };

    int yylex();
    void yyerror(struct exploitpattern* nm, char const *s);
    extern FILE* yyin;
    extern int yylineno;
%}

%union {
    struct str_array* arr;
    struct exploitpattern* xp;
    struct statement* st;
    char* string;
}

%parse-param { struct exploitpattern* xp }

%type <arr> exploitlist exploitdecl
%type <string> exploitgroup
%type <string> relop addop equality operator direction number value
%type <st> statement

%type <xp> exploit


%token <string> IDENTIFIER INT FLOAT
%token <string> EQ NEQ GT LT GEQ LEQ PLUSEQ SUBEQ
%token <string> ONEDIR ONEDIRBACK BIDIR
%token <string> INSERT UPDATE DELETE
%token EXPLOIT GLOBAL PRECONDITIONS POSTCONDITIONS
        GROUP COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE LPAREN RPAREN;

%%

root: exploitlist {}
;

exploitlist: { $$ = NULL; }
| exploitlist exploit {
    if($1 == NULL) {
        $$ = new_str_array();
        char* sql = make_exploit($2);
        add_str($$, sql);
    } else {
        char* sql = make_exploit($2);
        add_str($1, sql);
        $$ = $1;
    }
}
;

exploit: exploitdecl EXPLOIT IDENTIFIER LPAREN parameterlist RPAREN EQ options preconditions postconditions PERIOD {
    str_array* exploitopt = $1;
    char* exploitname = $3;

    str_array* params
}
;

exploitdecl: GLOBAL exploitgroup {
    $$ = new_str_array();
    add_str($$, $1);
    add_str($$, $2);
}
| exploitgroup {
    $$ = new_str_array();
    add_str($$, $1);
}
;

exploitgroup: GROUP LPAREN IDENTIFIER RPAREN { $$ = $3; }
| { $$ = NULL; }
;

parameterlist: parameters { $$ = $1; }
| { $$ = NULL; }
;

parameters: IDENTIFIER COMMA parameters {
    if($3 == NULL) {
        $$ = new_str_array();
        add_str($$, $1);
    } else {
        $$ = $3;
        add_str($$, $1);
    }
}
| IDENTIFIER {}
;

options: options statement SEMI {}
| statement SEMI {}
| {}
;

preconditions: PRECONDITIONS COLON preconditionslist {}
;

preconditionslist: preconditionslist precondition {}
| precondition {}
;

precondition: fact {}
;

postconditions: POSTCONDITIONS COLON postconditionslist {}
;

postconditionslist: postconditionslist postcondition {}
| postcondition {}
;

postcondition: operation fact {}
;

operation: INSERT {}
| DELETE {}
| UPDATE {}
;

fact: QUALITY COLON IDENTIFIER COMMA statement SEMI {}
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI {}
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

value: IDENTIFIER
| number
;

number: INT
| FLOAT
;

operator: relop
| addop
| equality
| ONEDIRBACK
;

equality: EQ
| NEQ
;

relop: GT
| LT
| GEQ
| LEQ
;

addop: PLUSEQ
| SUBEQ
;

direction: ONEDIR
| ONEDIRBACK
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

    struct exploitpattern xp;

    //yydebug = 1;
    yyin = file;
    do {
        yyparse(&xp);
    } while(!feof(yyin));

    //printf("%s : %d\n", "flowmeter", get_hashtable(nm.asset_tab, "flowmeter"));
    //print_str_array(nm.assets);
    //print_str_array(nm.facts);
}

void yyerror(struct exploitpattern* xp, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

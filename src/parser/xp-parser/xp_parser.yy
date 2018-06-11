%debug
%error-verbose

%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "util/mem.h"
    #include "util/str_array.h"
    #include "util/hash.h"
    #include "util/build_sql.h"
    #include "util/list.h"

    #define YYDEBUG 1

    int yylex();
    void yyerror(struct list *xplist, char const *s);
    extern FILE* yyin;
    extern int yylineno;
%}

%union {
    struct str_array* arr;
    struct list* list;
    struct exploitpattern* xp;
    struct exploitdecl* ed;
    struct statement* st;
    struct postcondition* pc;
    char* string;
}

%parse-param { struct list *xplist }

%type <arr> parameters options preconditions preconditionslist
%type <list> exploitlist postconditions postconditionslist
%type <ed> exploitdecl
%type <string> exploitgroup
%type <string> relop addop equality operator direction number value operation
%type <string> fact precondition
%type <pc> postcondition
%type <st> statement
%type <xp> exploit

%token <string> IDENTIFIER INT FLOAT
%token <string> EQ NEQ GT LT GEQ LEQ PLUSEQ SUBEQ
%token <string> ONEDIR ONEDIRBACK BIDIR
%token <string> INSERT UPDATE DELETE
%token <string> GLOBAL
%token EXPLOIT PRECONDITIONS POSTCONDITIONS GROUP COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY WHITESPACE LPAREN RPAREN;

%%

root: exploitlist {}
;

exploitlist:
| exploitlist exploit {
    list_add(xplist, $2);
}
;

exploit: exploitdecl EXPLOIT IDENTIFIER LPAREN parameters RPAREN EQ options preconditions postconditions PERIOD {
    struct exploitpattern* xp = getmem(sizeof(struct exploitpattern));
    xp->name = $3;
    xp->global = $1->global;
    xp->group = $1->group;
    xp->params = $5;
    xp->options = $8;
    xp->preconditions = $9;
    xp->postconditions = $10;
    $$ = xp;
}
;

exploitdecl: GLOBAL exploitgroup {
    struct exploitdecl *ed = getmem(sizeof(struct exploitdecl));
    ed->global = 1;
    if($2 != NULL) {
        ed->group = dynstr($2);
    } else {
        ed->group = NULL;
    }
    $$ = ed;
}
| exploitgroup {
    struct exploitdecl *ed = getmem(sizeof(struct exploitdecl));
    ed->global = 0;
    if($1 != NULL) {
        ed->group = dynstr($1);
    } else {
        ed->group = NULL;
    }
    $$ = ed;
}
;

exploitgroup: GROUP LPAREN IDENTIFIER RPAREN { $$ = $3; }
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
| IDENTIFIER {
    $$ = new_str_array();
    add_str($$, $1);
}
| { $$ = NULL; }
;

options: options statement SEMI {
    if($1 == NULL) {
        $$ = new_str_array();

        size_t stringlen = strlen($2->obj) + strlen($2->op) + strlen($2->val);
        char *statementstr = getstr(stringlen);
        strncat(statementstr, $2->obj, stringlen);
        strncat(statementstr, $2->op, stringlen);
        strncat(statementstr, $2->val, stringlen);

        add_str($$, statementstr);
    } else {
        size_t stringlen = strlen($2->obj) + strlen($2->op) + strlen($2->val);
        char *statementstr = getstr(stringlen);
        strncat(statementstr, $2->obj, stringlen);
        strncat(statementstr, $2->op, stringlen);
        strncat(statementstr, $2->val, stringlen);

        add_str($1, statementstr);

        $$ = $1;
    }
}
| { $$ = NULL; }
;

preconditions: PRECONDITIONS COLON preconditionslist {
    $$ = $3;
}
;

preconditionslist: { $$ = NULL; }
| preconditionslist precondition {
    if($1 == NULL) {
        $$ = new_str_array();
        add_str($$, $2);
    } else {
        add_str($1, $2);
        $$ = $1;
    }
}
;

precondition: fact {
    $$ = $1;
}
;

postconditions: POSTCONDITIONS COLON postconditionslist {
    $$ = $3;
}
;

postconditionslist: { $$ = NULL; }
| postconditionslist postcondition {
    if($1 == NULL) {
        $$ = list_new();
        list_add($$, $2);
    } else {
        list_add($1, $2);
        $$ = $1;
    }
}
;

postcondition: operation fact {
    struct postcondition *pc = getmem(sizeof(struct postcondition));
    pc->op = $1;
    pc->fact = $2;

    $$ = pc;
}
;

operation: INSERT { $$ = $1; }
| DELETE { $$ = $1; }
| UPDATE { $$ = $1; }
;

fact: QUALITY COLON IDENTIFIER COMMA statement SEMI {
    char buf[100];
    snprintf(buf, 100, "quality:%s,%s%s%s;", $3, $5->obj, $5->op, $5->val);
    $$ = dynstr(buf);
}
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA statement SEMI {
    char buf[100];
    snprintf(buf, 100, "topology:%s%s%s,%s%s%s;", $3, $4, $5, $7->obj, $7->op, $7->val);
    $$ = dynstr(buf);
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

void print_xp_list(struct list *xplist) {
    for(int i=0; i<xplist->size; i++) {
        struct exploitpattern *xp = list_get_idx(xplist, i);
        printf("Exploit: %s\n", xp->name);

        if(xp->global == 1) {
            printf("\tglobal: true\n");
        }

        if(xp->group != NULL) {
            printf("\tgroup: %s\n", xp->group);
        }

        printf("\tParams:\n");
        for(int j=0; j<xp->params->used; j++) {
            printf("\t\t%s\n", get_str_idx(xp->params, j));
        }

        if(xp->options != NULL) {
            printf("\tOptions:\n");
            for(int j=0; j<xp->options->used; j++) {
                printf("\t\t%s\n", get_str_idx(xp->options, j));
            }
        }

        printf("\tPreconditions:\n");
        for(int j=0; j<xp->preconditions->used; j++) {
            printf("\t\t%s\n", get_str_idx(xp->preconditions, j));
        }

        printf("\tPostconditions:\n");
        for(int j=0; j<xp->postconditions->size; j++) {
            struct postcondition *pc = list_get_idx(xp->postconditions, j);
            printf("\t\t%s %s\n", pc->op, pc->fact);
        }
    }
}

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

    struct list *xplist = list_new();

    //yydebug = 1;
    yyin = file;
    do {
        yyparse(xplist);
    } while(!feof(yyin));

    print_xp_list(xplist);
    char *sql = make_exploit(xplist);
}

void yyerror(struct list *xplist, char const *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    exit(-1);
}

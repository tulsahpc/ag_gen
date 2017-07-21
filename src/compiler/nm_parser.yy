%error-verbose

%{
    #include <stdio.h>

    #define YYDEBUG 0

    int yylex();
    void yyerror(char const *s);
    extern FILE* yyin;
    extern int yylineno;
%}

%union {
    char* string;
    int num;
    double fnum;
}
%token <string> IDENTIFIER
%token <num> INT
%token <fnum> FLOAT
%token NETWORK MODEL EQ ASSETS COLON FACTS PERIOD SEMI QUALITY COMMA TOPOLOGY ONEDIR BIDIR WHITESPACE;
%token GT LT GEQ LEQ
%%

root:
| NETWORK IDENTIFIER EQ ASSETS COLON assetlist FACTS COLON factlist PERIOD
;

assetlist:
| assetlist IDENTIFIER SEMI
;

factlist:
| factlist fact
;

fact:
  QUALITY COLON IDENTIFIER COMMA statement SEMI
| TOPOLOGY COLON IDENTIFIER direction IDENTIFIER COMMA IDENTIFIER SEMI
;

statement:
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
  EQ
| relop
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
    FILE* file = fopen("examples/grammar-test.nm", "r");
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
}

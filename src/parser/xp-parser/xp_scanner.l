%option noyywrap
%option yylineno

DIGIT   -?[0-9]+
ID      [a-zA-Z][a-zA-Z0-9_\.]*

%{
#include "xp_parser.tab.h"
%}

%%

#.*\n {}

global { return GLOBAL; }
group { return GROUP; }
exploit { return EXPLOIT; }
preconditions { return PRECONDITIONS; }
postconditions { return POSTCONDITIONS; }
quality  { return QUALITY; }
topology  { return TOPOLOGY; }
insert { return INSERT; }
update { return UPDATE; }
delete { return DELETE; }

{DIGIT}+ { yylval.string = strdup(yytext); return INT; }
{DIGIT}*"."{DIGIT}+ { yylval.string = strdup(yytext); return FLOAT; }
{ID} { yylval.string = strdup(yytext); return IDENTIFIER; }

"(" { yylval.string = strdup(yytext); return LPAREN; }
")" { yylval.string = strdup(yytext); return RPAREN; }
"="	 { yylval.string = strdup(yytext); return EQ; }
"!=" { yylval.string = strdup(yytext); return NEQ; }
"<" { yylval.string = strdup(yytext); return LT; }
">" { yylval.string = strdup(yytext); return GT; }
">=" { yylval.string = strdup(yytext); return GEQ; }
"<=" { yylval.string = strdup(yytext); return LEQ; }
":="  { yylval.string = strdup(yytext); return EQ; }
"<>" { yylval.string = strdup(yytext); return NEQ; }
"+=" { yylval.string = strdup(yytext); return PLUSEQ; }
"-=" { yylval.string = strdup(yytext); return SUBEQ; }
":"	 { yylval.string = strdup(yytext); return COLON; }
";"	 { yylval.string = strdup(yytext); return SEMI; }
","	 { yylval.string = strdup(yytext); return COMMA; }
"."	 { yylval.string = strdup(yytext); return PERIOD; }
"->" { yylval.string = strdup(yytext); return ONEDIR; }
"<-" { yylval.string = strdup(yytext); return ONEDIRBACK; }
"<->"  { yylval.string = strdup(yytext); return BIDIR; }
[ \t\r\n]+ {}

. { return yytext[0]; }

%%

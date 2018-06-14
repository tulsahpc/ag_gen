%option noyywrap
%option yylineno

DIGIT   -?[0-9]+
ID      [a-zA-Z][a-zA-Z0-9_\.]*

%{
#include "xp_parser.tab.h"
%}

%%

#.*\n {}

exploit { yylval.string = strdup(yytext); return EXPLOIT; }
preconditions { yylval.string = strdup(yytext); return PRECONDITIONS; }
postconditions { yylval.string = strdup(yytext); return POSTCONDITIONS; }
quality  { yylval.string = strdup(yytext); return QUALITY; }
topology  { yylval.string = strdup(yytext); return TOPOLOGY; }
insert { yylval.string = strdup(yytext); return INSERT; }
update { yylval.string = strdup(yytext); return UPDATE; }
delete { yylval.string = strdup(yytext); return DELETE; }

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
"!>" { yylval.string = strdup(yytext); return NOTONEDIR; }
"<!>" { yylval.string = strdup(yytext); return NOTBIDIR; }
[ \t\r\n]+ {}

. { return yytext[0]; }

%%

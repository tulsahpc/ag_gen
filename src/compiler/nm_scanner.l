%option noyywrap
%option yylineno

DIGIT   [0-9]+
ID      [a-zA-Z][a-zA-Z0-9_]*

%{
#include "nm_parser.tab.h"
%}

%%

#.*\n {}

network  { return NETWORK; }
facts  { return FACTS; }
assets  { return ASSETS; }
quality  { return QUALITY; }
topology  { return TOPOLOGY; }

{DIGIT}+ { yylval.string = yytext; return INT; }
{DIGIT}*"."{DIGIT}+ { yylval.string = yytext; return FLOAT; }
{ID} { yylval.string = yytext; return IDENTIFIER; }

"="	 { return EQ; }
"<" { return LT; }
">" { return GT; }
">=" { return GEQ; }
"<=" { return LEQ; }
":="  { return EQ; }
":"	 { return COLON; }
";"	 { return SEMI; }
","	 { return COMMA; }
"."	 { return PERIOD; }
"->"  { return ONEDIR; }
"<->"  { return BIDIR; }
[ \t\r\n]+ {}

. { return yytext[0]; }

%%

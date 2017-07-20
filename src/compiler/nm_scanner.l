%option noyywrap

DIGIT   [1-9][0-9]*
ID      [a-zA-Z][a-zA-Z0-9]*

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

{DIGIT}+ { yylval.num = atoi(yytext); return INT; }
{DIGIT}+"."{DIGIT}* { yylval.fnum = atof(yytext); return FLOAT; }
{ID} { yylval.string = (char*)malloc(32); return IDENTIFIER; }

"="	 { return EQ; }
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


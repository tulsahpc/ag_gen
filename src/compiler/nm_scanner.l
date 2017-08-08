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

"="	 { yylval.string = yytext; return EQ; }
"<" { yylval.string = yytext; return LT; }
">" { yylval.string = yytext; return GT; }
">=" { yylval.string = yytext; return GEQ; }
"<=" { yylval.string = yytext; return LEQ; }
":="  { yylval.string = yytext; return EQ; }
":"	 { yylval.string = yytext; return COLON; }
";"	 { yylval.string = yytext; return SEMI; }
","	 { yylval.string = yytext; return COMMA; }
"."	 { yylval.string = yytext; return PERIOD; }
"->"  { yylval.string = yytext; return ONEDIR; }
"<->"  { yylval.string = yytext; return BIDIR; }
[ \t\r\n]+ {}

. { return yytext[0]; }

%%

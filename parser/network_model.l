%option prefix="net_mod" //does the prefix affect the use of yylval?
%option outfile="lex.network_model.c" //wasn't sure on the outfile either?
%option noyywrap
/*definitions*/
%{
#include <math.h>
%}
%% /*flex rules*/
network	{yylval.sval = strdup(yytext); return NETWORK;}
assets	{yylval.sval = strdup(yytext); return ASSETS;}
facts	{yylval.sval = strdup(yytext); return FACTS;}
quality	{yylval.sval = strdup(yytext); return QUALITY;}
topology	{yylval.sval = strdup(yytext); return TOPOLOGY;}

[A-Za-z][A-Za-z0-9_]*	{yylval.sval = strdup(yytext); return IDENTIFIER;}
[A-Za-z0-9_]*   {yylval.sval = strdup(yytext); return STRING;}
[1-9][0-9]*     {yylval.ival = atoi(yytext); return NUMBER;}
"="     { return EQLOP;}
[><]=?  { return RELOP;}
->|<->  { return DIRECTION;}
[ \t\r\n]+	{ return WHITESPACE;} /* whitespace */

.	{printf( "Unrecognized character: (%s)\n", yytext); return ERR;}
%%

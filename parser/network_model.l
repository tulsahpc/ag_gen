%option prefix="net_mod" //does the prefix affect the use of yylval?
%option outfile="lex.network_model.c" //wasn't sure on the outfile either?
%option noyywrap
/*definitions*/
%{
%}
%% /*flex rules*/
asset	{yylval.sval = strdup(yytext); return ASSET;}
fact	{yylval.sval = strdup(yytext); return FACT;}
quality	{yylval.sval = strdup(yytext); return QUALITY;}
topology	{yylval.sval = strdup(yytext); return TOPOLOGY;}
[A-Za-z][A-Za-z0-9_]*	{yylval.sval = strdup(yytext); return ID;}
"="	{ return EQ;}
":"	{ return COLON;}
";"	{ return SEMI;}
","	{ return COMMA;}
"."	{ return PERIOD;}
"(" { return LPAREN;}
")" { return RPAREN;}
[ \t\r\n]+	{} /* whitespace */

.	{printf( "Unrecognized character: (%s)\n", yytext); return ERR;}
%%

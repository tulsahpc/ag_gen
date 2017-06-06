%option prefix="ns"
%option outfile="network_state.c"
%option noyywrap
/*definitions*/
%{/* need this for the call to atof() below */
#include <math.h>
#include "network_state_parser.tab.h"
%}
ID       [A-Za-z][A-Za-z0-9_]*
%% /*flex rules*/
network	{return NETWORK;}
model	{return MODEL;}
facts	{return FACTS;}
assets	{return ASSETS;}
quality	{return QUALITY;}
topology	{return TOPOLOGY;}
{ID}	{nslval.i = strdup(nstext); return IDENTIFIER;} 
"="	{return EQ;}
":"	{return COLON;}
";"	{return SEMI;}
","	{return COMMA;}
"."	{return PERIOD;}
[ \t\r\n]+	{} /* eat up whitespace */

.	{printf( "Unrecognized character: (%s)\n", nstext); return -1;}
%% /* C Code*/
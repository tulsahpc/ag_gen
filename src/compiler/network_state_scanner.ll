%option c++
%option outfile="network_state_scanner.cpp"
%option noyywrap

%{
#include <iostream>
#include "network_state_parser.tab.hpp"
%}

ID        [A-Za-z][A-Za-z0-9_]*

%%

#.*\n {}

network { return NETWORK; }
model { return MODEL; }
facts { return FACTS; }
assets { return ASSETS; }
quality { return QUALITY; }
topology { return TOPOLOGY; }

{ID} { return IDENTIFIER; }

"="	{ return EQ; }
":"	{ return COLON; }
";"	{ return SEMI; }
","	{ return COMMA; }
"."	{ return PERIOD; }
[ \t\r\n]+	{}

. { printf( "Unrecognized character: (%s)\n", yytext); return -1; }

%%

// int main(int argc, char** argv) {
//     FlexLexer* lexer = new yyFlexLexer;
//     while(lexer->yylex() != 0) {
//         std::cout << lexer->YYText() << std::endl;
//     }
//     return 0;
// }

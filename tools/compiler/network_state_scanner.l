%option c++
%option prefix="ns"
%option outfile="network_state_scanner.cpp"
%option noyywrap

ID        [A-Za-z][A-Za-z0-9_]*
NETWORK   network
MODEL     model
FACTS     facts
ASSETS    assets
QUALITY   quality
TOPOLOGY  topology

%%

{ ID } { return IDENTIFIER; }

"="	{ return EQ; }
":"	{ return COLON; }
";"	{ return SEMI; }
","	{ return COMMA; }
"."	{ return PERIOD; }
[ \t\r\n]+	{}

. { printf( "Unrecognized character: (%s)\n", nstext); return -1; }

%%

int main(int argc, char** argv) {
    FlexLexer* lexer = new yyFlexLexer;
    while(lexer->yylex() != 0);
    return 0;
}

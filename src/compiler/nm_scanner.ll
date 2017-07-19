%option c++ noyywrap nounput batch debug noinput yylineno
%option outfile="nm_scanner.cpp"
%option yyclass="NM::NM_Scanner"

%{
#include <iostream>
#include <fstream>
#include <string>

#include "nm_scanner.hpp"

#undef yywrap
#define yywrap() 1

#undef YY_DECL
#define YY_DECL int NM::NM_Scanner::yylex(const NM::NM_Parser::semantic_type* lval, NM::NM_Parser::location_type* loc)
#define yyterminate() return NM::NM_Parser::token::END
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

static NM::location loc;
%}

DIGIT   [1-9][0-9]*
ID      [a-zA-Z][a-zA-Z0-9]*

%%

%{
    loc->step();
%}

#.*\n {}

network  { return NM::NM_Parser::token::NETWORK; }
facts  { return NM::NM_Parser::token::FACTS; }
assets  { return NM::NM_Parser::token::ASSETS; }
quality  { return NM::NM_Parser::token::QUALITY; }
topology  { return NM::NM_Parser::token::TOPOLOGY; }

{DIGIT}+ { return NM::NM_Parser::token::INTEGER; }
{DIGIT}+"."{DIGIT}* { return NM::NM_Parser::token::FLOAT; }
{ID} { return NM::NM_Parser::token::IDENTIFIER; }

"="	 { return NM::NM_Parser::token::EQ; }
":="  { return NM::NM_Parser::token::EQ; }
":"	 { return NM::NM_Parser::token::COLON; }
";"	 { return NM::NM_Parser::token::SEMI; }
","	 { return NM::NM_Parser::token::COMMA; }
"."	 { return NM::NM_Parser::token::PERIOD; }
"->"  { return NM::NM_Parser::token::ONEDIR; }
"<->"  { return NM::NM_Parser::token::BIDIR; }
[ \t\r\n]+	 {}

. { std::cerr << loc << ": " << "Unknown symbol" << std::endl; }

%%

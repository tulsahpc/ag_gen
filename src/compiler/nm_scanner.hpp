#ifndef NM_SCANNER_HPP
#define NM_SCANNER_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "nm_parser.tab.hpp"
#include "location.hh"

namespace NM {
    class NM_Scanner : public yyFlexLexer {
    public:
        NM_Scanner(std::istream& in) : yyFlexLexer(&in) {};

        using FlexLexer::yylex;
        virtual int yylex(const NM::NM_Parser::semantic_type* lval,
            NM::NM_Parser::location_type* loc);
    private:
        NM::NM_Parser::semantic_type* yylval = nullptr;
    };
}

#endif //NM_SCANNER_HPP

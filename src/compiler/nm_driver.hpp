#ifndef NMDRIVER_HPP
#define NMDRIVER_HPP

#include <string>
#include "nm_scanner.hpp"
#include "nm_parser.tab.hpp"

namespace NM {
    class NM_Driver {
    public:
        void parse(const std::string& f);
        void error(const NM::NM_Parser::location_type& l, const std::string& err_message);

        std::string file;
    };
}

#endif

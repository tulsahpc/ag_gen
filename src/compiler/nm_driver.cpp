#include <fstream>
#include "nm_driver.hpp"
#include "nm_scanner.hpp"
#include "nm_parser.tab.hpp"

void NM::NM_Driver::parse(const std::string& f) {
    std::ifstream stream(f);
    NM::NM_Scanner scanner(stream);
    NM::NM_Parser parser(scanner, *this);
    parser.parse();
}

// int main(int argc, char** argv) {
//     std::ifstream filestream;
//     filestream.open("../../examples/SystemV8.nm");
//     FlexLexer* lexer = new yyFlexLexer;
//     lexer->switch_streams(&filestream);
//     while(lexer->yylex() != 0) {
//         std::cout << lexer->YYText() << std::endl;
//     }
//     return 0;
// }

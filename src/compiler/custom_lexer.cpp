//
// Created by Kyle Cook on 7/12/17.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "custom_lexer.h"

using namespace std;

Lexer::Lexer(string filename) {
	ifstream file;
	file.open(filename);

	vector<Token> tokens;

	char c = (char)file.get();
	while(!file.eof()) {
		// Eat Whitespace
		while(c == ' ' || c == '\t') {
			c = (char)file.get();
		}

		// New Lines
		if(c == '\n') {
			Token tok{NEWLINE, ""};
			tokens.push_back(tok);

			// Eat extra unnecessary newlines
			while(c == '\n') {
				c = (char)file.get();
			}
			continue;
		}

		// Comments
		if(c == '#') {
			Token tok{HASH, ""};
			tokens.push_back(tok);
			c = (char)file.get();
			continue;
		}

		// Symbols
		// =, ->, ",", ".", ";"
		if(c == '=') {
			Token tok{EQ, ""};
			tokens.push_back(tok);
			c = (char)file.get();
			continue;
		}

		// Numbers
		if(is_digit(c)) {
			ostringstream lexeme;
			bool isFloat = false;

			lexeme.put(c);
			while(is_digit(c) || c == '.') {
				if(c == '.') isFloat = true;
				c = (char)file.get();
				lexeme.put(c);
			}

			if(isFloat) {
				// Floating point number
				Token tok{FLOAT, lexeme.str()};
				tokens.push_back(tok);
			} else {
				// Integer
				Token tok{INT, lexeme.str()};
				tokens.push_back(tok);
			}
			continue;
		}

		// Letters
		if(is_alpha(c)) {

		}
	}
}

Token Lexer::next_tok(void) {

}

int main(int argc, char** argv) {
	Lexer lex("test.txt");
}

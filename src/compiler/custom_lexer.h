//
// Created by Kyle Cook on 7/12/17.
//

#ifndef AG_GEN_CUSTOM_LEXER_H
#define AG_GEN_CUSTOM_LEXER_H

#include <cctype>

enum TokenType {
	NETWORK,
	ASSETS,
	QUALITY,
	TOPOLOGY,
	IDENTIFIER,
	EQ,
	COLON,
	SEMI,
	COMMA,
	PERIOD,
	NEWLINE,
	HASH,
	INT,
	FLOAT
};

struct Token {
	TokenType type;
	std::string value;
};

class Lexer {
	bool is_digit(char c) {
		return std::isdigit(static_cast<unsigned char>(c));
	}

	bool is_alpha(char c) {
		return std::isalpha(static_cast<unsigned char>(c));
	}

	bool is_alnum(char c) {
		return std::isalnum(static_cast<unsigned char>(c));
	}
public:
	Lexer(std::string);
	Token next_tok(void);
};


#endif //AG_GEN_CUSTOM_LEXER_H

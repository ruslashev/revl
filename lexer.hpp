#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

enum token_k {
	TOKEN_WORD, TOKEN_INTEGER,
	TOKEN_OPENING_PAREN, TOKEN_CLOSING_PAREN, TOKEN_EQUALS,
	TOKEN_COMMA
};
struct token_t {
	token_k kind;

	std::string word;
	int integer;
};

std::vector<token_t> lex(std::string file);

#endif


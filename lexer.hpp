#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

enum token_k {
	TOKEN_WORD, TOKEN_INTEGER,
	TOKEN_OPENING_PAREN, TOKEN_CLOSING_PAREN, TOKEN_EQUALS,
	TOKEN_COMMA, TOKEN_EOF
};
struct token {
	token_k kind;

	std::string word;
	int integer;
};

std::vector<token> lex(std::string file);

#endif


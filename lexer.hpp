#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

enum token_k { TOKEN_OPENING_BRACE, TOKEN_CLOSING_BRACE, TOKEN_WORD, TOKEN_INTEGER };
struct token_t {
	token_k kind;
	std::string word;
	int integer;
};

std::vector<token_t> lex(std::string file);

#endif


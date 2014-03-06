#include "lexer.hpp"

std::vector<token_t> lex(std::string file)
{
	std::vector<token_t> tokens;

	for (char c : file) {
		printf("%c\n", c);
	}

	return tokens;
}


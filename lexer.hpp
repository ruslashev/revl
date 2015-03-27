#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

enum token_k {
	TOKEN_WORD,
	TOKEN_INTEGER,
	TOKEN_OPENING_PAREN,
	TOKEN_CLOSING_PAREN,
	TOKEN_EQUALS,
	TOKEN_COMMA,
	TOKEN_EOF
};
struct token {
	token_k kind;

	std::string word;
	int integer;

	int line, column;

	void print();
};

class Lexer
{
	std::string file;

	bool _is_alpha(char ch);
	bool _is_digit(char ch);
	bool _is_punct(char ch);

	void check_parentheses(std::vector<token> _tokens);
public:
	std::vector<token> Lex(std::string nfile);
};

#endif


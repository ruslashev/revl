#include "lexer.hpp"
#include "utils.hpp"

void token::print()
{
	switch (kind) {
		case TOKEN_WORD:
			printf("WORD: %s\n", word.c_str());
			break;
		case TOKEN_INTEGER:
			printf("INTEGER: %d\n", integer);
			break;
		case TOKEN_OPENING_PAREN:
			printf("OPENING_PAREN\n");
			break;
		case TOKEN_CLOSING_PAREN:
			printf("CLOSING_PAREN\n");
			break;
		case TOKEN_COMMA:
			printf("COMMA\n");
			break;
		case TOKEN_EQUALS:
			printf("EQUALS\n");
			break;
		case TOKEN_EOF:
			printf("EOF\n");
			break;
	}
}

bool Lexer::_is_alpha(char ch)
{
	return
		(ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z');
}

bool Lexer::_is_digit(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool Lexer::_is_punct(char ch)
{
	return
		ch == '(' ||
		ch == ')' ||
		ch == '=' ||
		ch == ',';
}

std::vector<token> Lexer::Lex(std::string nfile)
{
	file = nfile;

	std::vector<token> _tokens;
	int i = 0;
	char ch = file[i++];

	while (i < file.length()) {
		std::string word_buf = "";
		std::string integer_buf = "";

		while (_is_alpha(ch)) {
			word_buf += ch;
			ch = file[i++];
		}
		while (_is_digit(ch)) {
			integer_buf += ch;
			ch = file[i++];
		}
		if (word_buf != "") {
			token new_token { TOKEN_WORD };
			new_token.word = word_buf;
			_tokens.push_back(new_token);
			continue;
		} else if (integer_buf != "") {
			token new_token { TOKEN_INTEGER };
			new_token.integer = std::stoi(integer_buf);
			_tokens.push_back(new_token);
			continue;
		}

		if (_is_punct(ch)) {
			token new_token;
			switch (ch) {
				case '(': new_token.kind = TOKEN_OPENING_PAREN; break;
				case ')': new_token.kind = TOKEN_CLOSING_PAREN; break;
				case '=': new_token.kind = TOKEN_EQUALS; break;
				case ',': new_token.kind = TOKEN_COMMA; break;
			}
			_tokens.push_back(new_token);
		}

		ch = file[i++];
	}

	_tokens.push_back(token { TOKEN_EOF });

	checkParens(_tokens);

	return _tokens;
}

void Lexer::checkParens(std::vector<token> _tokens)
{
	int balance = 0;

	for (auto &t : _tokens)
		if (t.kind == TOKEN_OPENING_PAREN)
			balance++;
		else if (t.kind == TOKEN_CLOSING_PAREN)
			balance--;

	if (balance == 0)
		return;

	int line = 1, col = 1;
	struct pos_pair { int line, col; };
	std::vector<pos_pair> opening_parens;
	for (auto i = 0; i < file.size(); i++) {
		if (file[i] == '(')
			opening_parens.push_back({ line, col });
		else if (file[i] == ')') {
			if (opening_parens.size() == 0)
				error("ERROR: Mismatched closing parenthesis at "
						"line %d, column %d", line, col);
			else
				opening_parens.pop_back();
		} else if (file[i] == '\n') {
			line++;
			col = 0;
		}
		col++;
	}
	error("ERROR: Mismatched opening parenthesis at "
			"line %d, column %d",
			opening_parens.front().line, opening_parens.front().col);
}


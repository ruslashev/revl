#include "lexer.hpp"
#include "utils.hpp"

bool alpha(char ch)
{
	return (ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z');
}
bool digit(char ch)
{
	return ch >= '0' && ch <= '9';
}
bool punct(char ch)
{
	return ch == '(' ||
		ch == ')' ||
		ch == '[' ||
		ch == ']' ||
		ch == '=' ||
		ch == '.' ||
		ch == ',';
}

std::vector<token> lex(std::string file)
{
	std::vector<token> tokens;
	int i = 0;
	char ch = file[i++];
	int parensBalance = 0;

	while (i < file.length()) {
		token newToken;
		if (alpha(ch)) {
			std::string buf = "";
			while (alpha(ch)) {
				buf += ch;
				ch = file[i++];
			}
			newToken.kind = TOKEN_WORD;
			newToken.word = buf;
			tokens.push_back(newToken);
			continue;
		}

		if (digit(ch)) {
			std::string strBuf = "";
			while (digit(ch)) {
				strBuf += ch;
				ch = file[i++];
			}
			newToken.kind = TOKEN_INTEGER;
			newToken.integer = std::stoi(strBuf);
			tokens.push_back(newToken);
			continue;
		}

		if (punct(ch)) {
			switch (ch) {
				case '(': newToken.kind = TOKEN_OPENING_PAREN; parensBalance++; break;
				case ')': newToken.kind = TOKEN_CLOSING_PAREN; parensBalance--; break;
				case ',': newToken.kind = TOKEN_COMMA; break;
				case '=': newToken.kind = TOKEN_EQUALS; break;
				default: continue;
			}
			ch = file[i++];
			tokens.push_back(newToken);
			continue;
		}

		ch = file[i++];
	}

	if (parensBalance != 0) {
		int line = 1, col = 1;
		struct pos_pair { int line, col; };
		std::vector<pos_pair> openingParens;
		for (i = 0; i < file.length(); i++) {
			if (file[i] == '(') {
				openingParens.push_back({ line, col });
			} else if (file[i] == ')') {
				if (openingParens.size() == 0)
					error("ERROR: Mismatched closing parenthesis at line %d, column %d",
							line, col);
				else
					openingParens.pop_back();
			} else if (file[i] == '\n') {
				line++;
				col = 0;
			}
			col++;
		}
		error("ERROR: Mismatched opening parenthesis at line %d, column %d",
				openingParens.front().line, openingParens.front().col);
	}

	return tokens;
}


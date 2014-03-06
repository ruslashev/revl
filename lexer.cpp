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

std::vector<token_t> lex(std::string file)
{
	std::vector<token_t> tokens;
	int i = 0;
	char ch = file[i++];
	int braceBalance = 0;

	while (i < file.length()) {
		token_t newToken;
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
				case '(': newToken.kind = TOKEN_OPENING_BRACE; braceBalance++; break;
				case ')': newToken.kind = TOKEN_CLOSING_BRACE; braceBalance--; break;
				case '=': newToken.kind = TOKEN_EQUALS; break;
				default: continue;
			}
			ch = file[i++];
			tokens.push_back(newToken);
			continue;
		}

		ch = file[i++];
	}

	if (braceBalance != 0) {
		int line = 0, col = 0;
		struct pos_pair { int line, col; };
		std::vector<pos_pair> openingBraces;
		for (i = 0; i < file.length(); i++) {
			if (file[i] == '(') {
				openingBraces.push_back({ line, col });
			} else if (file[i] == ')') {
				if (openingBraces.size() != 0)
					openingBraces.pop_back();
			} else if (file[i] == '\n') {
				line++;
				col = 0;
			}
			col++;
		}
		error("ERROR: Mismatched brace at line %d, column %d\n", line, col);
	}

	return tokens;
}


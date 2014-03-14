#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "utils.hpp"

int main()
{
	std::string fileContents = readFile("test.revl");

	std::vector<token_t> tokens;
	tokens = lex(fileContents);

	puts("\nFinished lexing ..\n");

	for (auto &t : tokens) {
		switch (t.kind) {
			case TOKEN_WORD:
				printf("WORD: %s\n", t.word.c_str());
				break;
			case TOKEN_INTEGER:
				printf("INTEGER: %d\n", t.integer);
				break;
			case TOKEN_OPENING_BRACE:
				printf("OPENING_BRACE\n");
				break;
			case TOKEN_CLOSING_BRACE:
				printf("CLOSING_BRACE\n");
				break;
			case TOKEN_COMMA:
				printf("COMMA\n");
				break;
			case TOKEN_EQUALS:
				printf("EQUALS\n");
				break;
		}
	}

	node root = parse(tokens);
	puts("\nFinished parsing ..\n");
	root.print(0);

	return 0;
}


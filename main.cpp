#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "utils.hpp"

int main()
{
	std::string fileContents = readFile("test.revl");

	std::vector<token> tokens;
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

	node root = parse(tokens);
	puts("\nFinished parsing ..\n");
	root.print(0);

	return 0;
}


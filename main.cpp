#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "utils.hpp"

int main()
{
	puts("Reading file..\n");
	std::string fileContents = readFile("test.revl");
	printf("File contents: \"%s\"\n", fileContents.c_str());
	puts("\nFile read.\n");

	puts("Lexing..\n");
	std::vector<token> tokens;
	tokens = lexer_lex(fileContents);
	for (auto &t : tokens)
		t.print();
	puts("\nFinished lexing..\n");

	puts("Parsing..\n");
	node root = parser_parse(tokens);
	root.print(0);
	puts("\nFinished parsing..\n");

	puts("Evaluating..\n");
	int result = evaluator_evaluate(root);
	printf("\nFinished evaluating. Result: %d\n", result);

	return 0;
}


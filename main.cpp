#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "utils.hpp"

int main()
{
	std::string fileContents = readFile("test.revl");

	std::vector<token> tokens;
	tokens = lex(fileContents);

	puts("\nFinished lexing..\n");
	for (auto &t : tokens)
		t.print();

	node root = parse(tokens);
	puts("\nFinished parsing..\n");
	root.print(0);

	int result = evaluate(root);
	printf("Result: %d\n", result);

	return 0;
}


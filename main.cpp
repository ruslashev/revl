#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "utils.hpp"

int main()
{
	puts("1. Reading file");
	std::string fileContents = ReadFile("test.revl");
	printf("File contents: \"%s\"\n", fileContents.c_str());
	puts("File read.");

	puts("2. Lexing...");
	std::vector<token> tokens;
	Lexer lexer;
	tokens = lexer.Lex(fileContents);
	for (auto &t : tokens)
		t.print();
	puts("Finished lexing.");

	puts("3. Parsing..");
	Parser parser;
	Node root = parser.Parse(tokens);
	root.Print(0);
	puts("Finished parsing.");

	puts("4. Evaluating...");
	int result = evaluator_evaluate(root);
	printf("Finished evaluating. Result = %d\n", result);

	return 0;
}


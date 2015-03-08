#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>
#include <memory>

enum node_k {
	NODE_PROGRAM,
	NODE_CONSTANT,
	NODE_WORD,
	NODE_FUNCION_CALL,
	NODE_EXPRESSION,
	NODE_FUNCTION_DEFINITION,
	NODE_STUB
};

struct Node {
	node_k kind;

	int constant_number;

	std::string word_word;

	Node *function_call_function;
	Node *function_call_argument;

	std::string function_definition_name;
	std::vector<std::string> function_definition_arguments;

	std::vector<Node*> next;

	void Print(int indent);
	void PrintType();
};

class Parser
{
	token *_current;

	void next_token();
	Node* parse_definition();
	Node* parse_expression();
	Node* parse_function_call();
public:
	Parser();
	Node Parse(const std::vector<token> &ntokens);
};

#endif


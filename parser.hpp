#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>
#include <memory>
#include <cstdarg>

enum node_k {
	NODE_PROGRAM,
	NODE_CONSTANT,
	NODE_WORD,
	NODE_FUNCTION_CALL,
	NODE_EXPRESSION,
	NODE_FUNCTION_DEFINITION,
	NODE_CONSTANT_DEFINITION,
	NODE_STUB
};

struct Node {
	node_k kind;

	int constant_number;

	std::string word_word;

	Node *function_call_function; // alias for next[0]
	Node *function_call_argument; // for next[1]

	std::string function_definition_name;
	std::vector<std::string> function_definition_arguments;

	std::string constant_definition_name;

	std::vector<Node*> next;

	void Print(int indent);
	void PrintType();
};

Node* create_node(node_k nkind);

class Parser
{
	token *_current;

	void next_token();
	token* peek_next_token();
	Node* parse_constant_definition();
	Node* parse_expression();
	Node* parse_function_call();
	Node* parse_function_definition();

	void error(const char *format, ...);
public:
	Parser();
	Node Parse(const std::vector<token> &ntokens);
};

#endif


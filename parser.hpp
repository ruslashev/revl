#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>
#include <memory>

enum node_k {
	NODE_ROOT,
	NODE_DEFINITION,
	NODE_EXPERESSION,
	NODE_CONSTANT,
	NODE_DEFINITION_CALL
};

class Node {
public:
	node_k kind;
	std::string definitonName;
	std::vector<std::string> definitionArgList;
	int constValue;
	std::vector<Node*> next;

	void Print(int indent);
	void PrintType();
};

class Parser
{
	token *_current;

	void nextToken();
	Node* parseExpression();
	Node* parseDefinition();
public:
	Parser();
	Node Parse(std::vector<token> tokens);
};

#endif


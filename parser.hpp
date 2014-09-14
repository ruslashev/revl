#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>

enum node_k {
	NODE_ROOT,
	NODE_DEFINITION,
	NODE_EXPERESSION,
	NODE_CONSTANT,
	NODE_DEFINITION_CALL
};
struct node {
	node_k kind;
	std::vector<node*> next;

	std::string definitonName;
	int constValue;
	std::vector<std::string> definitionArgList;

	void print(int indent);
	void printType();
};

node parser_parse(std::vector<token> tokens);

#endif


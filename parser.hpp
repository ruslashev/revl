#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>

enum node_k {
	NODE_DEFINITION_ARGUMENTLESS, NODE_DEFINITION, NODE_CONSTANT, NODE_ROOT
};
struct node {
	node_k kind;
	std::vector<node*> next;

	std::string defName;
	int constValue;
	std::vector<std::string> definitionArgList;

	void print(int indent);
};

node parse(std::vector<token> tokens);

#endif


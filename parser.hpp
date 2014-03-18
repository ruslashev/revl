#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>

enum node_k {
	NODE_ROOT, NODE_EXPERESSION,
	NODE_DEFINITION_ARGUMENTLESS, NODE_DEFINITION, NODE_CONSTANT,
	NODE_VARIABLE_FETCH
};
struct node {
	node_k kind;
	std::vector<node*> next;

	std::string definitonName;
	int constValue;
	std::vector<std::string> definitionArgList;

	void print(int indent);
};

node parse(std::vector<token> tokens);

#endif


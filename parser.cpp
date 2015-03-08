#include "parser.hpp"
#include "utils.hpp"

/* alpha  = 'A' .. 'Z' | 'a' .. 'z' ;
 * punct = '(' | ')' | '=' | ',' ;
 * digit = [0-9] ;
 *
 * integer = digit, { digit };
 * word = alpha, { alpha } ;
 *
 * constant = integer | function_call ;
 * function_call = '(', word, [ expression ], ')'
 *
 * expression = function_call | constant | word ;
 *
 * function_definition = word, word, [ { ',', word } ], '=', expression
 *
 * program = { function_definition };
 */

static std::vector<std::unique_ptr<Node>> allocated_nodes;

Node* create_node(node_k nkind)
{
	Node *new_node = new Node;
	new_node->kind = nkind;

	new_node->function_call_argument = NULL;

	allocated_nodes.push_back(std::unique_ptr<Node>(new_node));
	return new_node;
}

Parser::Parser()
{
	_current = NULL;
}

void Parser::next_token()
{
	if (_current->kind != TOKEN_EOF)
		_current++;
	else
		error("Unexpected program termination");
}

#if 0
Node* Parser::parseExpression()
{
	Node *exprNode = create_node(NODE_EXPERESSION);

	if (_current->kind == TOKEN_INTEGER) {
		Node *constNode = create_node(NODE_CONSTANT);
		constNode->constValue = _current->integer;
		exprNode->next.push_back(constNode);
	} else if (_current->kind == TOKEN_WORD) {
		const std::string definedName = _current->word;
		nextToken();
		if (_current->kind == TOKEN_OPENING_PAREN) {
			Node *funcCall = create_node(NODE_DEFINITION_CALL);
			funcCall->definitonName = definedName;
			nextToken();
			while (1) {
				if (_current->kind == TOKEN_CLOSING_PAREN)
					break;
				funcCall->next.push_back(parseExpression());
				if (_current->kind == TOKEN_CLOSING_PAREN)
					break;
				if (_current->kind != TOKEN_COMMA)
					error("Failed to parse arguments for call of function \"%s\"",
							definedName.c_str());
				nextToken();
			}
			exprNode->next.push_back(funcCall);
		} else {
			Node *varFetch = create_node(NODE_DEFINITION_CALL);
			varFetch->definitonName = definedName;
			exprNode->next.push_back(varFetch);
		}
	} else {
		printf("Error: unknown expression: ");
		_current->print();
	}

	nextToken();
	return exprNode;
}
#endif

Node* Parser::parse_definition()
{
	Node *node = create_node(NODE_FUNCTION_DEFINITION);
	node->function_definition_name = _current->word;

	next_token();

	if (_current->kind == TOKEN_OPENING_PAREN) {
		next_token();
		if (_current->kind == TOKEN_CLOSING_PAREN)
			next_token();
		else {
			while (1) {
				if (_current->kind != TOKEN_WORD)
					error("Error: expected variable in definition for function "
							"\"%s\"", node->function_definition_name.c_str());
				node->function_definition_arguments.push_back(_current->word);
				next_token();
				if (_current->kind == TOKEN_CLOSING_PAREN) {
					next_token();
					break;
				} else if (_current->kind == TOKEN_COMMA) {
					next_token();
					continue;
				} else
					error("Error: variables in function definition must be "
							"separated by commas.");
			}
		}
	}

	if (_current->kind == TOKEN_EQUALS)
		node->next.push_back(create_node(NODE_STUB));
	else
		error("Error: expected '=' in function definition for \"%s\"",
				node->function_definition_name.c_str());

	return node;
}

Node Parser::Parse(const std::vector<token> &ntokens)
{
	std::vector<token> tokens = ntokens;

	Node root;
	root.kind = NODE_PROGRAM;

	_current = &tokens.front();

	while (_current->kind != TOKEN_EOF) {
		if (_current->kind == TOKEN_WORD)
			root.next.push_back(parse_definition());
		else {
			break;
			error("Unexpected top level token, expected word");
		}
	}

	return root;
}

void Node::Print(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("Node ");

	PrintType();

	if (next.size() == 0) {
		printf("\n");
	} else {
		printf(" --> (\n");
		for (auto &n : next)
			n->Print(indent+1);
		for (int i = 0; i < indent; i++)
			printf("\t");
		printf(")\n");
	}
}

void Node::PrintType()
{
	if (kind == NODE_PROGRAM) {
		printf("PROGRAM");
	} else if (kind == NODE_CONSTANT) {
		printf("CONSTANT");
	} else if (kind == NODE_WORD) {
		printf("WORD");
	} else if (kind == NODE_FUNCION_CALL) {
		printf("FUNCION_CALL");
	} else if (kind == NODE_EXPERESSION) {
		printf("EXPERESSION");
	} else if (kind == NODE_FUNCTION_DEFINITION) {
		printf("FUNCTION_DEFINITION %s ", function_definition_name.c_str());
		if (function_definition_arguments.size() == 0)
			printf("(no arguments)");
		else {
			std::vector<std::string> init = function_definition_arguments;
			init.pop_back();
			std::string last = function_definition_arguments.back();
			printf("(");
			for (auto &arg : init)
				printf("%s, ", arg.c_str());
			printf("%s)", last.c_str());
		}
	} else if (kind == NODE_STUB) {
		printf("STUB");
	}
}


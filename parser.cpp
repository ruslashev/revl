#include "parser.hpp"

/* alpha  = 'A' .. 'Z' | 'a' .. 'z' ;
 * punct = '(' | ')' | '=' | ',' ;
 * digit = [0-9] ;
 *
 * integer = digit, { digit };
 * word = alpha, { alpha } ;
 *
 * constant = integer | function_call ;
 * function_call = '(', word, [ expression ], ')' ;
 *
 * expression = constant | word | function_call ;
 *
 * constant_definition = word, '=', expression ;
 * function_definition = word, '(', word, { word }, ')', '=', expression ;
 *
 * program = { function_definition | constant_definition } ;
 */

void Node::PrintType()
{
	if (kind == NODE_PROGRAM) {
		printf("PROGRAM");
	} else if (kind == NODE_CONSTANT) {
		printf("CONSTANT %d", constant_number);
	} else if (kind == NODE_WORD) {
		printf("WORD %s", word_word.c_str());
	} else if (kind == NODE_FUNCTION_CALL) {
		printf("FUNCTION_CALL");
	} else if (kind == NODE_EXPRESSION) {
		printf("EXPRESSION");
	} else if (kind == NODE_FUNCTION_DEFINITION) {
		printf("FUNCTION_DEFINITION %s", function_definition_name.c_str());
		std::vector<std::string> init = function_definition_arguments;
		init.pop_back();
		std::string last = function_definition_arguments.back();
		printf("(");
		for (auto &arg : init)
			printf("%s, ", arg.c_str());
		printf("%s)", last.c_str());
	} else if (kind == NODE_CONSTANT_DEFINITION) {
		printf("CONSTANT_DEFINITION %s", constant_definition_name.c_str());
	} else if (kind == NODE_STUB) {
		printf("STUB");
	}
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

static std::vector<std::unique_ptr<Node>> allocated_nodes;

Node* create_node(node_k nkind)
{
	Node *new_node = new Node;
	new_node->kind = nkind;
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

token* Parser::peek_next_token()
{
	if (_current->kind == TOKEN_EOF)
		error("Unexpected program termination (peek)");

	return (_current+1);
}

Node* Parser::parse_function_call()
{ // expects TOKEN_OPENING_PAREN as a current token
	Node *node = create_node(NODE_FUNCTION_CALL);

	next_token();

	node->next.push_back(parse_expression());
	node->function_call_function = node->next[0];

	if (_current->kind == TOKEN_CLOSING_PAREN)
		error("Opening parenthesis implies a function call.\n"
				"Function calls are required to take one argument.");

	node->next.push_back(parse_expression());

	node->function_call_argument = node->next[1];

	return node;
}

Node* Parser::parse_expression()
{ // expects TOKEN_INTEGER, TOKEN_WORD or TOKEN_OPENING_PAREN as current token
	Node *node = create_node(NODE_EXPRESSION);

	if (_current->kind == TOKEN_INTEGER) {
		Node *node_const = create_node(NODE_CONSTANT);
		node_const->constant_number = _current->integer;
		node->next.push_back(node_const);
	} else if (_current->kind == TOKEN_WORD) {
		Node *node_word = create_node(NODE_WORD);
		node_word->word_word = _current->word;
		node->next.push_back(node_word);
	} else if (_current->kind == TOKEN_OPENING_PAREN) {
		node->next.push_back(parse_function_call());
	} else {
		printf("Error: unknown expression: ");
		_current->print();
	}

	next_token();

	return node;
}

Node* Parser::parse_constant_definition()
{ // expects TOKEN_WORD as a current token
	Node *node = create_node(NODE_CONSTANT_DEFINITION);
	node->constant_definition_name = _current->word;

	next_token();

	if (_current->kind != TOKEN_EQUALS) {
		error("Error: expected '=' in constant definition for \"%s\"",
				node->constant_definition_name.c_str());
	} else {
		next_token();
		node->next.push_back(parse_expression());
	}

	return node;
}

Node* Parser::parse_function_definition()
{ // expects TOKEN_WORD as a current token
	Node *node = create_node(NODE_FUNCTION_DEFINITION);
	node->function_definition_name = _current->word;

	next_token();

	if (_current->kind != TOKEN_OPENING_PAREN)
		error("that should not happen");

	next_token();
	if (_current->kind == TOKEN_CLOSING_PAREN)
		error("Function definitions without arguments are not permitted.\n"
				"Remove parentheses to make \"%s\" a constant, which you "
				"probably meant to do.", node->function_definition_name.c_str());

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

	if (_current->kind != TOKEN_EQUALS)
		error("Error: expected '=' in function definition for \"%s\"",
				node->function_definition_name.c_str());

	next_token();
	node->next.push_back(parse_expression());

	return node;
}

Node Parser::Parse(const std::vector<token> &ntokens)
{
	std::vector<token> tokens = ntokens;

	Node root;
	root.kind = NODE_PROGRAM;

	_current = &tokens.front();

	while (_current->kind != TOKEN_EOF) {
		if (_current->kind == TOKEN_WORD) {
			std::string definition_name = _current->word;
			if (peek_next_token()->kind == TOKEN_OPENING_PAREN)
				root.next.push_back(parse_function_definition());
			else
				root.next.push_back(parse_constant_definition());
		} else {
			error("Unexpected top level token, expected word");
		}
	}

	return root;
}

void Parser::error(const char *format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 256, format, args);
	printf("Parser error @ (%d:%d):\n%s\n",
			_current->line, _current->column, buffer);
	va_end(args);
	exit(1);
}


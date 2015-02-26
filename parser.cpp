#include "parser.hpp"
#include "utils.hpp"

/* word  = 'A' .. 'Z' | 'a' .. 'z' ;
 * punct = '(' | ')' | '[' | ']' | '=' | '.' | ',' ;
 * integer = digit except zero, { digit } ;
 * name = word, { word } ;
 * constant = integer // for now
 *
 * definition call = name, [ '(', [ expression, { ',', expression } ], ')' ]
 * expression = constant | definition call;
 *
 * definition = name, [ '(', [ name, { ',', name } ], ')' ], '=', expression;
 *
 * program = { definition };
 */

static std::vector<std::unique_ptr<Node>> allocated_nodes;

Node* createNode(node_k nkind)
{
	Node *newNode = new Node;
	newNode->kind = nkind;
	allocated_nodes.push_back(std::unique_ptr<Node>(newNode));
	return newNode;
}

Parser::Parser()
{
	_current = NULL;
}

void Parser::nextToken()
{
	if (_current->kind != TOKEN_EOF)
		_current++;
}

Node* Parser::parseExpression()
{
	Node *exprNode = createNode(NODE_EXPERESSION);

	if (_current->kind == TOKEN_INTEGER) {
		Node *constNode = createNode(NODE_CONSTANT);
		constNode->constValue = _current->integer;
		exprNode->next.push_back(constNode);
	} else if (_current->kind == TOKEN_WORD) {
		const std::string definedName = _current->word;
		nextToken();
		if (_current->kind == TOKEN_OPENING_PAREN) {
			Node *funcCall = createNode(NODE_DEFINITION_CALL);
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
			Node *varFetch = createNode(NODE_DEFINITION_CALL);
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

Node* Parser::parseDefinition()
{
	const std::string definedName = _current->word;
	Node *definitionNode = NULL;

	nextToken();

	switch (_current->kind) {
		case TOKEN_EQUALS:
			definitionNode = createNode(NODE_DEFINITION);
			definitionNode->definitonName = definedName;
			nextToken();
			definitionNode->next.push_back(parseExpression());
			break;
		case TOKEN_OPENING_PAREN:
			definitionNode = createNode(NODE_DEFINITION);
			definitionNode->definitonName = definedName;
			nextToken();
			if (_current->kind != TOKEN_CLOSING_PAREN)
			while (1) {
				if (_current->kind != TOKEN_WORD)
					error("Error: expected variable in definition for function \"%s\"",
							definedName.c_str());
				definitionNode->definitionArgList.push_back(_current->word);
				nextToken();
				if (_current->kind == TOKEN_CLOSING_PAREN)
					break;
				if (_current->kind != TOKEN_COMMA)
					error("Error: variables in function definition must be separated "
							"by commas.");
				nextToken();
			}
			nextToken();
			if (_current->kind != TOKEN_EQUALS)
				error("Error: expected '=' in function definition for \"%s\"",
						definedName.c_str());
			nextToken();
			definitionNode->next.push_back(parseExpression());
			break;
		default:
			error("Error: expected a sane definiton");
	}

	return definitionNode;
}

Node Parser::Parse(std::vector<token> ntokens)
{
	std::vector<token> tokens = ntokens;
	Node root;
	root.kind = NODE_ROOT;
	_current = &tokens.front();
	while (_current->kind != TOKEN_EOF) {
		switch (_current->kind) {
			case TOKEN_WORD:
				root.next.push_back(parseDefinition());
				break;
			default:
				printf("Error: ");
				_current->print();
				goto end;
		}
	}
end:
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
		for (auto &n : next) {
			n->Print(indent+1);
		}
		for (int i = 0; i < indent; i++)
			printf("\t");
		printf(")\n");
	}
}

void Node::PrintType()
{
	if (kind == NODE_DEFINITION) {
		printf("NODE_DEFINITION \"%s\" (", definitonName.c_str());
		if (definitionArgList.size() == 0)
			printf("no variables)");
		else {
			for (int i = 0; i < definitionArgList.size()-1; i++)
				printf("\"%s\", ", definitionArgList[i].c_str());
			printf("\"%s\") ", definitionArgList.back().c_str());
		}
	} else if (kind == NODE_CONSTANT)
		printf("NODE_CONSTANT %d", constValue);
	else if (kind == NODE_ROOT)
		printf("NODE_ROOT");
	else if (kind == NODE_EXPERESSION)
		printf("NODE_EXPERESSION");
	else if (kind == NODE_DEFINITION_CALL)
		printf("NODE_DEFINITION_CALL \"%s\"", definitonName.c_str());
}


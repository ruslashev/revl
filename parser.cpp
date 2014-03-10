#include "parser.hpp"
#include "utils.hpp"

/* // Does not account whitespace
 *
 * word  = 'A' .. 'Z' | 'a' .. 'z' ;
 * punct = '(' | ')' | '[' | ']' | '=' | '.' | ',' ;
 * integer = digit except zero, { digit } ;
 * name = word, { word } ;
 *
 * definition = name, '=', integer;
 * program = { definition };
 */

void node::print(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("Node ");
	if (kind == NODE_DEFINITION)
		printf("NODE_DEFINITION \"%s\"", defName.c_str());
	else if (kind == NODE_CONSTANT)
		printf("NODE_CONSTANT %d", constValue);
	else if (kind == NODE_ROOT)
		printf("NODE_ROOT");
	printf(" --> (\n");
	for (node *n : next) {
		n->print(indent+1);
	}
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf(")\n");
}

node *createNode(node_k nkind)
{
	node *newNode = new node;
	newNode->kind = nkind;
	return newNode;
}

token_t *CurrentToken = NULL;
std::vector<token_t> tokens;

void nextToken()
{
	if (CurrentToken != &tokens.back())
		CurrentToken++;
	else
		CurrentToken = NULL;
}

node* parseDefinition()
{
	const std::string definedName = CurrentToken->word;
	node *definitionNode = createNode(NODE_DEFINITION);
	definitionNode->defName = definedName;
	nextToken();

	if (CurrentToken->kind != TOKEN_EQUALS)
		error("expected '=' in function definition for \"%s\"", definedName.c_str());
	nextToken();

	if (CurrentToken->kind == TOKEN_INTEGER) {
		node *constNode = createNode(NODE_CONSTANT);
		constNode->constValue = CurrentToken->integer;
		definitionNode->next.push_back(constNode);
		nextToken();
	} else
		puts("whoa dude");

	return definitionNode;
}

node parse(std::vector<token_t> ntokens)
{
	tokens = ntokens;
	node root;
	root.kind = NODE_ROOT;
	CurrentToken = &tokens.front();
	while (CurrentToken != NULL) {
		switch (CurrentToken->kind) {
			case TOKEN_WORD:
				root.next.push_back(parseDefinition());
				break;
			default:
				break;
		}
	}
	return root;
}


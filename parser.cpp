#include "parser.hpp"
#include "utils.hpp"

/* // Does not account whitespace
 *
 * word  = 'A' .. 'Z' | 'a' .. 'z' ;
 * punct = '(' | ')' | '[' | ']' | '=' | '.' | ',' ;
 * integer = digit except zero, { digit } ;
 * name = word, { word } ;
 *
 * expression = integer;
 * definition = argumentless definition | function definition;
 * argumentless definition = name, '=', expression;
 * function definition     = name, '(', name, { ',', name }, ')', '=', expression;
 * program = { definition };
 */

token *CurrentToken = NULL;
std::vector<token> tokens;

node *createNode(node_k nkind)
{
	node *newNode = new node;
	newNode->kind = nkind;
	return newNode;
}

void nextToken()
{
	if (CurrentToken != &tokens.back())
		CurrentToken++;
	else
		CurrentToken = NULL;
}

node* parseExpression()
{
	node *exprNode = createNode(NODE_EXPERESSION);
	if (CurrentToken->kind == TOKEN_INTEGER) {
		node *constNode = createNode(NODE_CONSTANT);
		constNode->constValue = CurrentToken->integer;
		exprNode->next.push_back(constNode);
	} else
		puts("whoa dude chill");

	nextToken();
	return exprNode;
}

node* parseDefinition()
{
	const std::string definedName = CurrentToken->word;
	node *definitionNode = NULL;
	nextToken();

	switch (CurrentToken->kind) {
		case TOKEN_EQUALS:
			definitionNode = createNode(NODE_DEFINITION_ARGUMENTLESS);
			definitionNode->defName = definedName;
			nextToken();
			definitionNode->next.push_back(parseExpression());
			break;
		case TOKEN_OPENING_PAREN:
			definitionNode = createNode(NODE_DEFINITION);
			definitionNode->defName = definedName;
			nextToken();
			while (1) {
				if (CurrentToken->kind != TOKEN_WORD)
					error("Error: expected variable in function definition for \"%s\"",
						definedName.c_str());
				definitionNode->definitionArgList.push_back(CurrentToken->word);
				nextToken();
				if (CurrentToken->kind == TOKEN_CLOSING_PAREN)
					break;
				if (CurrentToken->kind != TOKEN_COMMA)
					error("Error: variables in function definition must be separated "
							"by commas.");
				nextToken();
			}
			nextToken();
			if (CurrentToken->kind != TOKEN_EQUALS)
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

node parse(std::vector<token> ntokens)
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

void node::print(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("Node ");
	if (kind == NODE_DEFINITION) {
		printf("NODE_DEFINITION \"%s\" (", defName.c_str());
		for (int i = 0; i < definitionArgList.size()-1; i++)
			printf("\"%s\", ", definitionArgList[i].c_str());
		printf("\"%s\") ", definitionArgList.back().c_str());
	}
	if (kind == NODE_DEFINITION_ARGUMENTLESS)
		printf("NODE_DEFINITION_ARGUMENTLESS \"%s\"", defName.c_str());
	else if (kind == NODE_CONSTANT)
		printf("NODE_CONSTANT %d", constValue);
	else if (kind == NODE_ROOT)
		printf("NODE_ROOT");
	else if (kind == NODE_EXPERESSION)
		printf("NODE_EXPERESSION");
	if (next.size() == 0) {
		printf("\n");
	} else {
		printf(" --> (\n");
		for (node *n : next) {
			n->print(indent+1);
		}
		for (int i = 0; i < indent; i++)
			printf("\t");
		printf(")\n");
	}
}


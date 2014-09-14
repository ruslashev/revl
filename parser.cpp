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

token *CurrentToken = NULL;
std::vector<token> tokens;

node* createNode(node_k nkind)
{
	node *newNode = new node;
	newNode->kind = nkind;
	return newNode;
}

void nextToken()
{
	if (CurrentToken->kind != TOKEN_EOF)
		CurrentToken++;
}

bool CurrentTokenIs(token_k tokenKind)
{
	// actually this function is not relevant anymore,
	// and every call to it should be replaced with
	// normal `if (CurrentToken->kind == ..)` check,
	// but with it everything looks a bit more pretty
	return (CurrentToken->kind == tokenKind);
}

node* parseExpression()
{
	node *exprNode = createNode(NODE_EXPERESSION);

	if (CurrentTokenIs(TOKEN_INTEGER)) {
		node *constNode = createNode(NODE_CONSTANT);
		constNode->constValue = CurrentToken->integer;
		exprNode->next.push_back(constNode);
	} else if (CurrentTokenIs(TOKEN_WORD)) {
		const std::string definedName = CurrentToken->word;
		nextToken();
		if (CurrentTokenIs(TOKEN_OPENING_PAREN)) {
			node *funcCall = createNode(NODE_DEFINITION_CALL);
			funcCall->definitonName = definedName;
			nextToken();
			while (1) {
				if (CurrentTokenIs(TOKEN_CLOSING_PAREN))
					break;
				funcCall->next.push_back(parseExpression());
				if (CurrentTokenIs(TOKEN_CLOSING_PAREN))
					break;
				if (!CurrentTokenIs(TOKEN_COMMA))
					error("Failed to parse arguments for call of function \"%s\"",
							definedName.c_str());
				nextToken();
			}
			exprNode->next.push_back(funcCall);
		} else {
			node *varFetch = createNode(NODE_DEFINITION_CALL);
			varFetch->definitonName = definedName;
			exprNode->next.push_back(varFetch);
		}
	} else {
		printf("Error: unknown expression: ");
		CurrentToken->print();
	}

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
			definitionNode = createNode(NODE_DEFINITION);
			definitionNode->definitonName = definedName;
			nextToken();
			definitionNode->next.push_back(parseExpression());
			break;
		case TOKEN_OPENING_PAREN:
			definitionNode = createNode(NODE_DEFINITION);
			definitionNode->definitonName = definedName;
			nextToken();
			if (!CurrentTokenIs(TOKEN_CLOSING_PAREN))
			while (1) {
				if (!CurrentTokenIs(TOKEN_WORD))
					error("Error: expected variable in definition for function \"%s\"",
							definedName.c_str());
				definitionNode->definitionArgList.push_back(CurrentToken->word);
				nextToken();
				if (CurrentTokenIs(TOKEN_CLOSING_PAREN))
					break;
				if (!CurrentTokenIs(TOKEN_COMMA))
					error("Error: variables in function definition must be separated "
							"by commas.");
				nextToken();
			}
			nextToken();
			if (!CurrentTokenIs(TOKEN_EQUALS))
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

node parser_parse(std::vector<token> ntokens)
{
	tokens = ntokens;
	node root;
	root.kind = NODE_ROOT;
	CurrentToken = &tokens.front();
	while (!CurrentTokenIs(TOKEN_EOF)) {
		switch (CurrentToken->kind) {
			case TOKEN_WORD:
				root.next.push_back(parseDefinition());
				break;
			default:
				printf("Error: ");
				CurrentToken->print();
				goto end;
		}
	}
end:
	return root;
}

void node::print(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("Node ");

	printType();

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

void node::printType()
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


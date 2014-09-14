#include "evaluator.hpp"
#include "utils.hpp"

int result = 0;
int evaluateExpression(node *tree);
int evaluateDefinition(node *tree);

int evaluateDefinitionCall(node *tree)
{
	printf("Definition call: ");
	tree->printType();
	puts("");

	int localResult = 0;
	if (tree->definitonName == "plus") {
		if (tree->next.size() <= 1)
			error("plus function cannot take less than 2 arguments");
		for (node *child : tree->next)
			if (child->kind != NODE_EXPERESSION)
				error("Does that even happen?");
			else
				localResult += evaluateExpression(child);
	} else
		error("Called unknown function \"%s\"", tree->definitonName.c_str());

	return localResult;
}

int evaluateExpression(node *tree)
{
	if (tree->next.size() == 0)
		error("that should not have happened.");

	for (node *child : tree->next) {
		switch (child->kind) {
			case NODE_DEFINITION:
				return evaluateDefinition(child);
			case NODE_CONSTANT:
				return child->constValue;
			case NODE_DEFINITION_CALL:
				return evaluateDefinitionCall(child);
				break;
			default:
				printf("Error: unhandled type: ");
				child->printType();
				error("\n");
		}
	}
}

int evaluateDefinition(node *tree)
{
	printf("Definition: ");
	tree->printType();
	puts("");
	return evaluateExpression(tree->next[0]);
}

int evaluator_evaluate(node tree)
{
	printf("In ");
	tree.printType();
	puts("");

	switch (tree.kind) {
		case NODE_ROOT:
			result = 0;
			for (node *child : tree.next)
				evaluator_evaluate(*child);
			return result;
			break;
		case NODE_DEFINITION:
			result = evaluateDefinition(&tree);
			return result;
			break;
		default:
			break;
	}
}


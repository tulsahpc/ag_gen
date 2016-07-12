#include <stdlib.h>
#include "util_list.h"
#include "util_stack.h"

struct Stack *StackNew()
{
	struct Stack *stack = malloc(sizeof(struct Stack));
	stack->list = malloc(sizeof(struct List));
	return stack;
}

void StackPush(struct Stack *stack, void *val)
{
	ListAppend(stack->list, val);
}

void *StackPop(struct Stack *stack)
{
	return ListPop(stack->list);
}

int StackSize(struct Stack *stack)
{
	return ListSize(stack->list);
}

void StackFree(struct Stack *stack)
{
	free(stack->list);
	free(stack);
}

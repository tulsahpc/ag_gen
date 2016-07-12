#ifndef C_UTIL_STACK_H
#define C_UTIL_STACK_H

struct Stack {
	struct List *list;
};

struct Stack *StackNew(void);
void StackPush(struct Stack *, void *);
void *StackPop(struct Stack *);
int StackSize(struct Stack *);
void StackFree(struct Stack *);

#endif //C_UTIL_STACK_H

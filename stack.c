
#include "stack.h"

Stack *initStack()
{
    Stack *stack;
    stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

void push(Stack *s, NodeStack *node)
{
    node->next = s->top;
    s->top = node;
}

NodeStack *pop(Stack *s)
{
    NodeStack *node = s->top;
    s->top = node->next;
    return node;
}

char *top(Stack *s)
{
    return s->top->command;
}

void print_stack(Stack *s)
{
    if (!s)
        return;

    for (NodeStack *it = s->top; it != NULL; it = it->next)
    {
        printf("%s", it->command);
    }
}

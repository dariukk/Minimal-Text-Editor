#include "editor.h"

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

void deleteStack(Stack *s)
{
    NodeStack *node;
    node = s->top;

    while (node)
    {
        NodeStack *p = node->next;

        free(node->command);
        free(node);

        node = p;
    }
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

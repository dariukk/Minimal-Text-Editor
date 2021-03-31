
#include "stack.h"

Stack *initStack()
{
    Stack *stack;
    stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

void push(Stack *s, char *command)
{
    NodeStack *node;
    node = (NodeStack *)malloc(sizeof(NodeStack *));

    node->command = (char *)malloc(sizeof(char) * 15);
    strcpy(node->command, command);
    node->next = s->top;
    s->top = node;
}

void pop(Stack *s)
{
    NodeStack *node = s->top;

    s->top = s->top->next;
    free(node);
}

char *top(Stack *s)
{
    return s->top->command;
}
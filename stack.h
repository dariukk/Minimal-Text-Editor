#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef NODESTACK
#define NODESTACK

// declararea stivei pentru comenzi
typedef struct NodeStack
{
    char *command;
    int num;
    struct NodeStack *next;
} NodeStack;

#endif

#ifndef STACK
#define STACK

typedef struct Stack
{
    NodeStack *top;
} Stack;

#endif

Stack *initStack();

void push(Stack *s, NodeStack *node);

NodeStack *pop(Stack *s);

char *top(Stack *s);

void print_stack(Stack *s);

void deleteStack(Stack *s);

#endif

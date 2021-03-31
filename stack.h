#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// declararea stivei pentru comenzi

typedef struct NodeStack
{
    char *command;
    struct NodeStack *next;
} NodeStack;

typedef struct Stack
{
    NodeStack *top;
} Stack;

Stack *initStack(); 

void push(Stack *s, char *command);

void pop(Stack *s);

char *top(Stack *s);
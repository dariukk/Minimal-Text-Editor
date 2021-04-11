#include "editor.h"

element newElement()
{
    element x;

    x.command = (char *)malloc(100 * sizeof(char));
    x.new = (char *)malloc(100 * sizeof(char));
    x.old = (char *)malloc(100 * sizeof(char));
    strcpy(x.new, "");
    strcpy(x.old, "");
    x.list = initList();
    x.num = 0;
    x.prevPos = 0;
    x.prevLine = 0;

    return x;
}

Stack *initStack()
{
    // initializez stiva stack
    Stack *stack;

    stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

void push(Stack *s, element value)
{
    // aduag un element in stiva
    NodeStack *node = (NodeStack *)malloc(sizeof(NodeStack));

    node->value = value;
    node->next = s->top;
    s->top = node;
}

void deleteNodeStack(NodeStack *node)
{
    free(node->value.command);
    free(node->value.new);
    free(node->value.old);
    deleteList(&node->value.list);
    free(node->value.list);
    free(node);
}

void pop(Stack *s)
{
    // elimin elementul din varful stivei
    NodeStack *node = s->top;
    s->top = s->top->next;
    deleteNodeStack(node);
}

element top(Stack *s)
{
    // returneaza valoarea elementului din varful stivei
    element value;

    value = newElement();
    strcpy(value.command, s->top->value.command);
    strcpy(value.new, s->top->value.new);
    strcpy(value.old, s->top->value.old);
    value.num = s->top->value.num;
    value.prevLine = s->top->value.prevLine;
    value.beginofLine = s->top->value.beginofLine;
    value.prevPos = s->top->value.prevPos;

    for (Node *node = s->top->value.list->head;
         node != NULL; node = node->next)
        insertCharacter(value.list, node->elem);

    return value;
}

void deleteStack(Stack *s)
{
    // sterg elementele din stiva s si eliberez memoria
    
    NodeStack *node;
    node = s->top;

    while (node)
    {
        NodeStack *p = node->next;
        deleteNodeStack(node);
        node = p;
    }
}

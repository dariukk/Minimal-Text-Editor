#include "editor.h"

element newElement()
{
    element x;

    x.command = (char *)malloc(100 * sizeof(char));
    x.list = initList();
    x.num = 0;

    return x;
}

Stack *initStack()
{
    Stack *stack;

    stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

void push(Stack *s, element value)
{
    NodeStack *node = (NodeStack *)malloc(sizeof(NodeStack));

    node->value = value;
    node->next = s->top;
    s->top = node;
}

void deleteNodeStack(NodeStack *node)
{
    free(node->value.command);
    deleteList(&node->value.list);
    free(node->value.list);
    free(node);
}

void pop(Stack *s)
{
    NodeStack *node = s->top;
    s->top = s->top->next;
    deleteNodeStack(node);
}

element top(Stack *s)
{
    element value;
    value = newElement();
    strcpy(value.command, s->top->value.command);
    value.num = s->top->value.num;

    for (Node *node = s->top->value.list->head; node != NULL; node = node->next)
        insertCharacter(value.list, node->elem);

    return value;
}

void deleteStack(Stack *s)
{
    NodeStack *node;
    node = s->top;

    while (node)
    {
        NodeStack *p = node->next;
        deleteNodeStack(node);
        node = p;
    }
}

void print_stack(Stack *s)
{
    if (!s)
        return;

    for (NodeStack *it = s->top; it != NULL; it = it->next)
    {
        printf("%s ", it->value.command);
    }
}

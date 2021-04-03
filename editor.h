#ifndef EDITOR_H_
#define EDITOR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef NODE
#define NODE

// declararea atat a listei pentru text
// cat si a copiei acesteia
typedef struct Node
{
    char elem;
    int line, pos;
    struct Node *next;
    struct Node *prev;
} Node;

#endif

#ifndef LISTTEXT
#define LISTTEXT

typedef struct ListText
{
    Node *head;
    Node *tail;
    Node *cursor;
} ListText;

#endif


#ifndef NODESTACK
#define NODESTACK

#define addtext "addtext"


// declararea stivei pentru comenzi
typedef struct NodeStack
{
    char *command;
    int num;
    ListText *list;
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

void print_stack(Stack *s);

void deleteStack(Stack *s);

ListText *initList();

void printList(ListText *list, FILE *out);

void insertCharacter(ListText *list, char elem);

void gotoLine(ListText *list, int line);

void gotoChar(ListText *list, int pos, int line);

void deleteLine(ListText *list, int line);

void save(ListText *list, ListText *finalList);

void backspace(ListText *list);

void delete (ListText *list, int num);

int getNum(char *s);

int digits(int num);

void undo(ListText *list, Stack *undoStack, Stack *redoStack);

void redo(ListText *list, Stack *undoStack, Stack *redoStack);

void deleteList(ListText **list);

void reorderLines(ListText *list);

char *getString(char *s);

void replace(ListText *list, char *old, char *new);

void deleteWord(ListText *list, char *word);

#endif

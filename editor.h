#ifndef EDITOR_H_
#define EDITOR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#define addtext "addtext"

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

void undo(ListText *list, Stack *stack);

#endif

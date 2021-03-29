#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// declararea atat a listei pentru text
// cat si a copiei acesteia
typedef struct Node
{
    char elem;
    int line, pos;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct ListText
{
    Node *head;
    Node *tail;
    Node *cursor;
} ListText;

// declararea stivei pentru comenzi
typedef struct Stack
{
    char *command;
    struct Stack *next;

} Stack;

void init();

void printList(ListText *list);

void insertCharacter(char elem);

int addText();

void save();

int doCommands();

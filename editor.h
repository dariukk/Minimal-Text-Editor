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

void printList(ListText *list,FILE *out);

void insertCharacter(ListText *list, char elem);

void gotoLine(ListText *list, int line);

void gotoChar(ListText *list, int pos, int line);

void deleteLine(ListText *list, int line);

void save(ListText *list, ListText *finalList);

void backspace(ListText *list);

void delete (ListText *list, int num);

int getNum(char *s);

int digits(int num);

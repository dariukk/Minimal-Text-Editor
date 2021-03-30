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

FILE *in, *out;

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

void printList(ListText *list)
{
    for (Node *node = list->head; node != NULL; node = node->next)
        fprintf(out, "%c", node->elem);

    fprintf(out, "\n");
}

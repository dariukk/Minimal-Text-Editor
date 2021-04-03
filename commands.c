
#include "editor.h"

void printList(ListText *list, FILE *out)
{
    for (Node *node = list->head; node != NULL; node = node->next)
        fprintf(out, "%c", node->elem);
}

void deleteList(ListText *list)
{
    // sterg toate elementele din lista

    Node *node;

    list->cursor = list->head;
    node = list->head;

    while (node)
    {
        Node *p;
        p = node;
        node = node->next;
        free(p);
    }
}

void insertCharacter(ListText *list, char elem)
{
    // inserez un nou caracter in lista dublu inlantuita
    Node *new_node;
    new_node = (Node *)malloc(sizeof(Node));
    new_node->elem = elem;

    if (list->tail == NULL)
    {
        // daca lista este goala elementul adaugat va fi primul element din lista
        new_node->prev = NULL;
        new_node->next = NULL;
        new_node->line = 1;
        new_node->pos = 1;
        list->head = new_node;
        list->tail = new_node;
        list->cursor = list->tail;
        return;
    }

    // daca mai exista elemente in lista
    // adaug caracterul la finalul listei
    list->tail->next = new_node;

    if (list->tail->elem == '\n')
    {
        new_node->line = list->tail->line + 1;
        new_node->pos = 1;
    }
    else
    {
        new_node->line = list->tail->line;
        new_node->pos = list->tail->pos + 1;
    }

    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->cursor = list->tail;
}

void gotoLine(ListText *list, int line)
{
    if (line == 1)
    {
        list->cursor = list->head;
        return;
    }

    Node *node = list->tail;
    while (!(node->line == line && node->prev->line == line - 1))
        node = node->prev;

    list->cursor = node;
}

void gotoChar(ListText *list, int pos, int line)
{
    Node *node = list->cursor;
    if (line != 0)
    {
        if (line > node->line)
            while (node->line != line && node->next)
                node = node->next;
        else if (line < node->line)
            while (node->line != line && node->prev)
                node = node->prev;
    }

    if (pos > node->pos)
        while (node->pos != pos && node->next)
            node = node->next;
    else if (pos < node->pos)
        while (node->pos != pos && node->prev)
            node = node->prev;

    list->cursor = node;
}

void deleteLine(ListText *list, int line)
{
    if (line == 1)
    {
        Node *node = list->head;

        while (node != NULL && node->line == 1)
        {
            Node *p = node;
            node = node->next;
        }

        return;
    }

    Node *node1, *node2;

    node1 = list->head;
    node2 = list->tail;

    while (node1 != NULL && !(node1->line == line - 1 && node1->next->line == line))
        node1 = node1->next;

    while (node2 != NULL && !(node2->line == line + 1 && node2->prev->line == line))
    {
        node2->line--;
        node2 = node2->prev;
    }

    Node *p = node1->next, *q = node2->prev;

    node1->next = node2;
    node2->prev = node1;

    while (p != q)
    {
        Node *aux = p;
        p = p->next;
        free(aux);
    }

    if (p != NULL)
        free(p);
}

void save(ListText *list, ListText *finalList)
{
    // copiez in finalList elementele listei list
    Node *finalNode;

    finalList->head = list->head;
    finalList->tail = finalList->tail;
    finalNode = finalList->head;

    for (Node *node = list->head; node->next != NULL; node = node->next)
    {
        finalNode->elem = node->elem;
        finalNode->next = node->next;
        finalNode->prev = node->prev;
        finalNode = finalNode->next;
    }
}

void backspace(ListText *list)
{
    if (list->cursor == list->tail)
    {
        Node *node = list->tail;
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
        list->cursor = list->tail;
        free(node);

        return;
    }

    Node *node = list->cursor;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    list->cursor = node->prev;
    free(node);
}

void delete (ListText *list, int num)
{
    if (num == 0)
        num = 1;

    Node *node = list->cursor->next;
    while (node && num)
    {
        --num;
        Node *p = node;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        node = node->next;
        free(p);
    }

    list->cursor = node->prev;
}

int getNum(char *s)
{
    int ans = 0, l = strlen(s), i = 0;
    while (s[i] >= '0' && s[i] <= '9' && i < l)
        ans = ans * 10 + (s[i] - '0'), ++i;

    return ans;
}

int digits(int num)
{
    int ans = 0;
    while (num)
    {
        ans++;
        num /= 10;
    }

    return ans;
}

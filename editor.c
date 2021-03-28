#include "editor.h"

FILE *in, *out;
Stack *stack;
ListText *finalList, *list;

void init()
{
    // initializez atat cele doua liste cat si stiva
    list = (ListText *)malloc(sizeof(ListText));
    list->head = NULL;
    list->cursor = NULL;
    list->tail = NULL;

    finalList = (ListText *)malloc(sizeof(ListText));
    finalList->head = NULL;
    finalList->cursor = NULL;
    finalList->tail = NULL;

    stack = (Stack *)malloc(sizeof(Stack));
}

void printList(ListText *list)
{
    for (Node *node = list->head; node != NULL; node = node->next)
        fprintf(out, "%c", node->elem);
}

void insertCharacter(char elem)
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
        list->head = new_node;
        list->tail = new_node;
        list->cursor = list->tail;
        return;
    }

    // daca mai exista elemente in lista
    // adaug caracterul la finalul listei
    list->tail->next = new_node;

    if (list->tail->elem == '\n')
        new_node->line = list->tail->line + 1;
    else
        new_node->line = list->tail->line;

    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->cursor = list->tail;
}

int addText()
{
    char chr;
    int ok = 0;
    while (fscanf(in, "%c", &chr))
    {
        if (chr != ':')
        {
            if (ok == 1)
                insertCharacter(':');
            insertCharacter(chr);
            ok = 0;
        }
        else
        {
            if (ok == 1)
            {
                // se trece la modul de inserare comenzi
                fscanf(in, "%c", &chr);
                return 1;
            }
            ok = 1;
        }
    }

    if (ok == 1)
        insertCharacter(':');

    return 0;
}

void gotoLine(int line)
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

void deleteLine(int line)
{
    Node *node1, *node2;

    node1 = list->head;
    node2 = list->tail;
    node2->line--;

    while (node1 != NULL 
    && !(node1->line == line - 1 && node1->next->line == line))
        node1 = node1->next;

    while (node2 != NULL && !(node2->line == line + 1 && node2->prev->line == line))
    {
        node2 = node2->prev;
        node2->line--;
    }

    /* free(node1->next);
    free(node2->prev);
    node1->next = node2;
    node2->prev = node1;*/
}

void save()
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

int getNum(char *s)
{
    int ans = 0;
    for (int i = 0; i < strlen(s); ++i)
        if (s[i] >= '0' && s[i] <= '9')
            ans = ans * 10 + (s[i] - '0');

    return ans;
}

int doCommands()
{
    char *command;
    command = (char *)malloc(100 * sizeof(char));

    // !!! de bagat comenzile in stiva

    while (fgets(command, 100, in))
    {
        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if (strcmp("::i", command) == 0)
        {
            // se trece la modul inserare text
            return 1;
        }
        else if (strcmp("q", command) == 0)
        {
            // se inchide editorul
            return 0;
        }
        else if (strcmp("s", command) == 0)
        {
            // se salveaza documentul
            save();
        }
        else if (command[0] == 'g' && command[1] == 'l')
        {
            // cursorul este mutat la inceputul liniei line
            int line = getNum(command);
            gotoLine(line);
        }
        else if (command[0] == 'd' && command[1] == 'l')
        {
            // se sterge linia line daca este introdusa
            // sau se sterge linia pe care se afla cursorul
            int line = getNum(command);

            if (line == 0)
                deleteLine(list->cursor->line);
            else
                deleteLine(line);
        }
    }

    return 0;
}

int main()
{
    init();

    in = fopen("editor.in", "r");
    out = fopen("editor.out", "w");

    int what = 1;
    while (1)
    {
        if (what == 1)
        {
            // mod inserare text
            // verific daca am ajuns la final de fisier
            // sau daca s-a inchis editorul
            if (!addText())
                break;
        }
        else
        {
            // mod inserare comenzi
            // verific daca am ajuns la final de fisier
            // sau daca s-a inchis editorul
            if (!doCommands())
                break;
        }
        what = 1 - what;
    }
    printList(finalList);

    fclose(in);
    fclose(out);
}

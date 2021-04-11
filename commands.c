#include "editor.h"

ListText *initList()
{
    // initializez lista

    ListText *list = (ListText *)malloc(sizeof(ListText));
    list->head = NULL;
    list->tail = NULL;
    list->cursor = list->tail;

    return list;
}

void printList(ListText *list, FILE *out)
{
    // afisez elementele listei list
    Node *node;

    for (node = list->head; node != NULL; node = node->next)
        fprintf(out, "%c", node->elem);
}

void reorderLines(ListText *list)
{
    Node *node = list->head;
    int line = 1, pos = 1;

    while (node)
    {
        node->line = line;
        node->pos = pos;

        if (node->elem == '\n')
            ++line, pos = 0;

        pos++;
        node = node->next;
    }
}

void deleteList(ListText **list)
{
    // sterg toate elementele din lista

    Node *node;
    node = (*list)->head;

    while (node)
    {
        Node *next;
        next = node->next;
        free(node);
        node = next;
    }

    (*list)->head = NULL;
    (*list)->tail = NULL;
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

    if (list->cursor == list->tail)
    {
        // daca mai exista elemente in lista
        // si cursorul este la final
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
        return;
    }

    list->cursor->next->prev = new_node;
    new_node->next = list->cursor->next;
    list->cursor->next = new_node;
    new_node->prev = list->cursor;

    list->cursor = new_node;

    reorderLines(list);
}

void deleteNewLines(ListText *list)
{
    // sterg liniile goale
    Node *node = list->head;

    while (node->next)
    {
        if (node->elem == '\n' && node->next->elem == '\n')
        {
            Node *p = node;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node = node->next;
            free(p);
        }
        node = node->next;
    }
}

// implementarea comenzii goto line (gl)
void gotoLine(ListText *list, int line, element nodeStack, int isUndo)
{
    // daca comanda nu este apelata in cadrul comenzii undo
    // retin pozitia anteriora a cursorului
    if (isUndo == 0)
    {
        nodeStack.prevLine = list->cursor->line;
        nodeStack.prevPos = list->cursor->pos;
    }

    // mut cursorul pe linia line
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

// implementarea functiei goto character (gc)
void gotoChar(ListText *list, int pos, int line, element *nodeStack, int isUndo)
{
    Node *node = list->cursor;

    // daca comanda nu este apelata in cadrul comenzii undo
    // retin pozitia anteriora a cursorului
    if (isUndo == 0)
    {
        nodeStack->prevLine = list->cursor->line;
        nodeStack->prevPos = list->cursor->pos;
    }

    // daca este specificata linia line
    // cursorul este mutat pe linia line
    if (line != 0)
    {
        if (line > node->line)
            while (node->line != line && node->next)
                node = node->next;
        else if (line < node->line)
            while (node->line != line && node->prev)
                node = node->prev;
    }

    // cursorul este mutat pe pozitia pos
    if (pos > node->pos)
        while (node->pos != pos && node->next)
            node = node->next;
    else if (pos < node->pos)
        while (node->pos != pos && node->prev)
            node = node->prev;

    list->cursor = node;
}

// implementarea comenzii delete line (dl)
void deleteLine(ListText *list, int line)
{
    // daca prima linie este stearsa
    if (line == 1)
    {
        Node *node = list->head;

        while (node != NULL && node->line == 1)
        {
            Node *p = node->next;
            free(node);
            node = p;
        }

        list->head = node;
        list->cursor = node;

        return;
    }

    // daca ultima linie este stearsa
    if (line == list->tail->line)
    {
        Node *node = list->tail;

        while (node != NULL && node->line == line)
        {
            Node *p = node->prev;
            free(node);
            node = p;
        }

        list->tail = node;
        list->cursor = node;

        return;
    }

    // daca linia care trebuie sa fie stearsa nu este nici prima nici ultima

    Node *node1, *node2;

    // node1 va fi nodul asociat ultimului caracter de pe linia line-1
    // node2 va fi nodul asociat primului caracter de pe linia line+1

    node1 = list->head;
    node2 = list->tail;

    while (node1 != NULL && !(node1->line == line - 1 &&
                              node1->next->line == line))
        node1 = node1->next;

    while (node2 != NULL && !(node2->line == line + 1 &&
                              node2->prev->line == line))
    {
        node2->line--;
        node2 = node2->prev;
    }

    Node *p = node1->next, *q = node2->prev;

    node1->next = node2;
    node2->prev = node1;
    list->cursor = node2;

    // eliberez memoria
    while (p != q)
    {
        Node *aux = p;
        p = p->next;
        free(aux);
    }

    if (p != NULL)
        free(p);
}

// implementarea comenzii save (s)
void save(ListText *list, ListText *finalList)
{
    // copiez in finalList elementele listei list

    deleteList(&finalList);

    for (Node *node = list->head; node != NULL; node = node->next)
        insertCharacter(finalList, node->elem);
}

// implementarea comenzii backspace (b)
void backspace(ListText *list, int isUndo)
{
    // daca cursorul se afla pe ultima pozitie din text
    if (list->cursor == list->tail)
    {
        if (list->tail->elem == '\n' && isUndo == 0)
        {
            // daca aplic operatia backspace pe text
            // voi sterge un caracter diferit de '\n'
            // acest lucru nu se aplica si la operatiile de undo

            Node *node = list->cursor->prev;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            list->cursor = node->prev;
            free(node);
            return;
        }

        Node *node = list->tail->prev;
        node->next = NULL;
        list->cursor = node;
        free(list->tail);
        list->tail = node;

        return;
    }

    // daca se aplica comanda backspace pe un element diferit de ultimul
    Node *node = list->cursor;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    list->cursor = node->prev;
    free(node);
}

// implementarea comenzii delete (d)
void delete (ListText *list, int num, int beginofLine, element nodeStack)
{
    if (num == 0)
        num = 1;

    // daca cursorul se afla la inceput de linie
    // comanda d va sterge incepand cu primul caracter de pe linie
    // altfel, comanda d va sterge incepand cu urmatorul caracter
    // in raport cu pozitia cursorului

    if (beginofLine == 1 && list->cursor->line == 1)
    {
        Node *node = list->head;

        while (node && num)
        {
            --num;
            Node *p = node;
            node = node->next;
            insertCharacter(nodeStack.list, p->elem);
            free(p);
        }

        list->cursor = node;
        list->head = node;
    }

    Node *node = list->cursor->next;

    if (beginofLine == 1)
        node = list->cursor;

    while (node && num)
    {
        --num;
        Node *p = node;
        insertCharacter(nodeStack.list, p->elem);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        node = node->next;
        free(p);
    }

    list->cursor = node->prev;

    if (beginofLine == 1)
        list->cursor = node;
}

// implementarea comenzii replace (re)
void replace(ListText *list, char *old, char *new)
{
    Node *node = list->cursor;

    // inlocuiesc prima aparitie a cuvantului old cu new
    while (node)
    {
        Node *aux = node;
        int i = 0, len = strlen(old);

        while (aux && i < len && aux->elem == old[i])
        {
            ++i;
            aux = aux->next;
        }

        if (i == len)
        {
            // am gasit prima aparitie a lui old

            Node *neww = (Node *)malloc(sizeof(Node)), *begin;
            neww->elem = new[0];
            begin = neww;

            i = 1;
            len = strlen(new);
            while (i < len)
            {
                Node *p = (Node *)malloc(sizeof(Node));
                p->elem = new[i];
                ++i;
                neww->next = p;
                p->prev = neww;
                neww = p;
            }

            node->prev->next = begin;
            begin->prev = node->prev;
            neww->next = aux;
            aux->prev = neww;

            // eliberez memoria
            Node *p = node;
            while (p != aux)
            {
                Node *next = p->next;
                free(p);
                p = next;
            }

            return;
        }

        node = node->next;
    }
}

// implementarea comenzii replace all (ra)
void replaceAll(ListText *list, char *old, char *new)
{
    Node *node = list->head;

    // inlocuiesc toate aparitiile cuvantului old cu new
    while (node)
    {
        Node *aux = node;
        int i = 0, len = strlen(old);

        while (aux && i < len && aux->elem == old[i])
        {
            ++i;
            aux = aux->next;
        }

        if (i == len)
        {
            // am gasit o aparitie a lui old

            Node *neww = (Node *)malloc(sizeof(Node)), *begin;
            neww->elem = new[0];
            begin = neww;

            i = 1;
            len = strlen(new);
            while (i < len)
            {
                Node *p = (Node *)malloc(sizeof(Node));
                p->elem = new[i];
                ++i;
                neww->next = p;
                p->prev = neww;
                neww = p;
            }

            if (node->line == 1 && node->pos == 1)
            {
                neww->next = aux;
                aux->prev = neww;

                if (list->head == list->cursor)
                    list->cursor = begin;

                list->head = begin;
            }
            else
            {
                node->prev->next = begin;
                begin->prev = node->prev;
                neww->next = aux;
                aux->prev = neww;
            }

            // eliberez memoria
            while (node != aux)
            {
                Node *next = node->next;
                free(node);
                node = next;
            }
        }
        else
            node = node->next;
    }
}

// implementarea comenzii delete word (dw)
void deleteWord(ListText *list, char *word, element *nodeStack)
{
    Node *node = list->cursor;

    // sterg prima aparitie a cuvantului word
    while (node)
    {
        Node *aux = node;
        int i = 0, len = strlen(word);

        while (aux && i < len && aux->elem == word[i])
        {
            ++i;
            aux = aux->next;
        }

        if (i == len)
        {
            // am gasit prima aparitie a lui word
            node->prev->next = aux;
            aux->prev = node->prev;

            if (list->cursor == node)
                list->cursor = aux;

            nodeStack->prevLine = aux->line;
            nodeStack->prevPos = aux->pos;

            // eliberez memoria
            Node *p = node;
            while (p != aux)
            {
                Node *next = p->next;
                free(p);
                p = next;
            }

            return;
        }

        node = node->next;
    }
}

// implementarea operatiei delete all (da)
void deleteAllWords(ListText *list, char *word)
{
    Node *node = list->head;

    // sterg toate aparitiile cuvantului word
    while (node)
    {
        Node *aux = node;
        int i = 0, len = strlen(word);

        while (aux && i < len && aux->elem == word[i])
        {
            ++i;
            aux = aux->next;
        }

        if (i == len)
        {
            // am gasit o aparitie a lui word
            if (node == list->head)
            {
                list->head = aux;
                aux->prev = NULL;
                return;
            }

            node->prev->next = aux;
            aux->prev = node->prev;

            if (node->prev->elem == '\n' && aux->elem == '\n')
            {
                node->prev->next = aux->next;
                aux->next->prev = node->prev;
                aux = aux->next;
            }

            // eliberez memoria
            while (node != aux)
            {
                Node *next = node->next;
                free(node);
                node = next;
            }
        }
        else
            node = node->next;
    }
}

// determin primul numar dintr-un sir
int getNum(char *s)
{
    int ans = 0, l = strlen(s), i = 0;
    while (s[i] >= '0' && s[i] <= '9' && i < l)
        ans = ans * 10 + (s[i] - '0'), ++i;

    return ans;
}

// determin numarul de cifre ale unui numar
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

// determin primul cuvant dintr-un sir
char *getString(char *s)
{
    char *ans = (char *)malloc(25 * sizeof(char));

    int i = 0, len = strlen(s);
    while (i < len && s[i] != ' ' && s[i] != '\n')
    {
        ans[i] = s[i];
        ++i;
    }

    ans[i] = '\0';
    return ans;
}

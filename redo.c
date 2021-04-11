#include "editor.h"

void redo(ListText **list, Stack **undoStack, Stack **redoStack)
{
    element value = top(*redoStack);
    pop(*redoStack);

    if (strcmp(value.command, addtext) == 0)
    {
        // redo addtext
        Node *p = value.list->tail;
        while (p)
        {
            insertCharacter(*list, p->elem);
            p = p->prev;
        }
    }
    else if (strcmp(value.command, del) == 0)
    {
        // redo delete
        int num = value.num;
        while (num)
        {
            backspace(*list, 1);
            --num;
        }
    }
    else if (strcmp(value.command, gochar) == 0 ||
             strcmp(value.command, goline) == 0)
    {
        // redo go to character si go to line
        int auxLine = (*list)->cursor->line, auxPos = (*list)->cursor->pos;
        gotoChar(*list, value.prevPos, value.prevLine, &value, 1);

        value.prevLine = auxLine;
        value.prevPos = auxPos;
    }
    else if (strcmp(value.command, deleteword) == 0)
    {
        // redo delete word
        gotoChar(*list, value.prevPos, value.prevLine, &value, 1);

        char *word = (char *)malloc(sizeof(char) * 20);
        int i = 0;
        Node *node = value.list->head;
        while (node)
        {
            word[i++] = node->elem;
            node = node->next;
        }

        word[i] = '\0';
        deleteWord(*list, word, &value);
        free(word);
    }
    else if (strcmp(value.command, simple_replace) == 0)
    {
        // redo replace
        char *aux = (char *)malloc(50 * sizeof(char));

        replace(*list, value.old, value.new);
        strcpy(aux, value.new);
        strcpy(value.new, value.old);
        strcpy(value.old, aux);

        free(aux);
    }

    push(*undoStack, value);
}

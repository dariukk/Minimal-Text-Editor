#include "editor.h"

void redo(ListText *list, Stack *undoStack, Stack *redoStack)
{
    element value = top(redoStack);
    pop(redoStack);

    if (strcmp(value.command, addtext) == 0)
    {
        Node *p = value.list->tail;
        while (p)
        {
            insertCharacter(list, p->elem);
            p = p->prev;
        }
    }
    else if (strcmp(value.command, del) == 0)
    {
        int num = value.num;
        while (num)
        {
            backspace(list, 1);
            --num;
        }
    }
    else if (strcmp(value.command, gochar) == 0)
    {
        int auxLine = list->cursor->line, auxPos = list->cursor->pos;
        gotoChar(list, value.prevPos, value.prevLine, &value, 1);

        value.prevLine = auxLine;
        value.prevPos = auxPos;
    }
    else if (strcmp(value.command, goline) == 0)
    {
        int auxLine = list->cursor->line;
        gotoLine(list, value.prevLine, value, 1);
        value.prevLine = auxLine;
    }

    push(undoStack, value);
}

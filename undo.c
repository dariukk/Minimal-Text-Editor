#include "editor.h"

void undo(ListText *list, Stack *undoStack, Stack *redoStack)
{
    element value = top(undoStack);
    pop(undoStack);

    if (strcmp(value.command, addtext) == 0)
    {
        int num = value.num;
        while (num)
        {
            insertCharacter(value.list, list->cursor->elem);
            backspace(list, 1);
            --num;
        }
    }
    else if (strcmp(value.command, del) == 0)
    {
        int num = value.num;
        Node *node = value.list->head;
        while (num)
        {
            insertCharacter(list, node->elem);
            node = node->next;
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

    push(redoStack, value);
}

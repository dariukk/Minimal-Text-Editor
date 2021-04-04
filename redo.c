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

    push(undoStack, value);
}

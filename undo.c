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
            backspace(list);
            --num;
        }
    }

    push(redoStack, value);
}

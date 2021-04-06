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

    push(redoStack, value);
}

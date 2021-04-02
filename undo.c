#include "editor.h"

void undo(ListText *list, Stack *undoStack, Stack *redoStack)
{
    NodeStack *node = pop(undoStack);

    node->list = initList();

    if (strcmp(node->command, addtext) == 0)
    {
        int num = node->num;
        while (num)
        {
            insertCharacter(node->list, list->cursor->prev->elem);
            backspace(list);
            --num;
        }
    }

    push(redoStack, node);
    free(node->command);
    free(node);
}

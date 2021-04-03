
#include "editor.h"

void redo(ListText *list, Stack *undoStack, Stack *redoStack)
{
    NodeStack *node = pop(redoStack);

    if (strcmp(node->command, addtext) == 0)
    {
        Node *p = node->list->tail;
        while (p)
        {
            insertCharacter(list, p->elem);
            p = p->prev;
        }
    }

    push(undoStack, node);
}

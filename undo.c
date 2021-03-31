#include "editor.h"
#include "stack.h"

void undo(ListText *list, Stack *stack)
{
    NodeStack *node = pop(stack);

    if (strcmp(node->command, addtext) == 0)
    {
        // pentru a da undo la comanda de inserare text
        // stergem toate
        
        int num = node->num;
        while (num)
        {
            backspace(list);
            --num;
        }
    }
}
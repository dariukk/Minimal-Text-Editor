#include "editor.h"

FILE *in, *out;
Stack *undoStack;
Stack *redoStack;

int addText(ListText *list)
{
    char chr;
    int ok = 0, multipleLines = 0;

    element node = newElement();
    strcpy(node.command, addtext);

    while (fscanf(in, "%c", &chr))
    {
        if (chr == '\n')
            multipleLines++;

        if (chr != ':')
        {
            if (ok == 1)
                insertCharacter(list, ':'), node.num++;
            insertCharacter(list, chr);

            node.num++;
            ok = 0;
        }
        else
        {
            if (ok == 1)
            {
                if (multipleLines <= 1)
                    if (list->cursor != list->tail)
                        backspace(list, 1);

                push(undoStack, node);

                // se trece la modul de inserare comenzi
                fscanf(in, "%c", &chr); // citesc caracterul 'i'

                return 1;
            }
            ok = 1;
        }
    }

    if (ok == 1)
        insertCharacter(list, ':'), node.num++;

    push(undoStack, node);

    return 0;
}

int doCommands(ListText *list, ListText *finalList)
{
    char *command;
    int beginofLine = 0;

    command = (char *)malloc(100 * sizeof(char));

    while (fgets(command, 100, in))
    {

        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if (command[0] == ':' && command[1] == ':' && command[2] == 'i')
        {
            // se trece la modul inserare text
            free(command);
            return 1;
        }
        else if (strcmp("q", command) == 0)
        {
            // se inchide editorul
            free(command);
            return 0;
        }
        else if (strcmp("s", command) == 0)
        {
            // se salveaza documentul
            save(list, finalList);
        }
        else if (command[0] == 'g' && command[1] == 'l')
        {
            // cursorul este mutat la inceputul liniei line
            int line = getNum(command + 3);
            gotoLine(list, line);
            beginofLine = 1;
        }
        else if (command[0] == 'd' && command[1] == 'l')
        {
            // se sterge linia line daca este introdusa
            // sau se sterge linia pe care se afla cursorul
            int line = getNum(command + 3);

            if (line == 0)
                deleteLine(list, list->cursor->line);
            else
                deleteLine(list, line);
        }
        else if (command[0] == 'g' && command[1] == 'c')
        {
            // cursorul este mutat pe pozitia introdusa
            // daca nu este indicata si o linie se va considera linia curenta

            int chr = getNum(command + 3);
            int line = getNum(command + 4 + digits(chr));

            gotoChar(list, chr, line);
            beginofLine = 0;
        }
        else if (command[0] == 'b')
        {
            //sterge caracterul de dinaintea cursorului
            backspace(list, 0);
        }
        else if (command[0] == 'd' && command[1] == 'w')
        {
            // se efectueaza operatia delete word
            char *word;
            word = getString(command + 3);

            deleteWord(list, word);
            free(word);
        }
        else if (command[0] == 'd' && command[1] == 'a')
        {
            // se efectueaza operatia delete word
            char *word;
            word = getString(command + 3);

            deleteAllWords(list, word);
            free(word);
        }
        else if (command[0] == 'd')
        {
            // sterge un numar de caractere incepand cu pozitia curenta
            element node = newElement();
            int num = getNum(command + 2);

            strcpy(node.command, del);
            node.num = num;
            node.beginofLine = beginofLine;
            if (node.num == 0)
                node.num = 1;

            delete (list, num, beginofLine, node);
            push(undoStack, node);
        }
        else if (command[0] == 'u')
        {
            // se efectueaza operatia de undo
            undo(list, undoStack, redoStack);
        }
        else if (command[0] == 'r' && command[1] == 'e')
        {
            // se efectueaza operatia replace
            char *old, *new;

            old = getString(command + 3);
            new = getString(command + 4 + strlen(old));

            replace(list, old, new);

            free(old);
            free(new);
        }
        else if (command[0] == 'r' && command[1] == 'a')
        {
            // se efectueaza operatia replace
            char *old, *new;

            old = getString(command + 3);
            new = getString(command + 4 + strlen(old));

            replaceAll(list, old, new);

            free(old);
            free(new);
        }
        else if (command[0] == 'r')
        {
            // se efectueaza operatia de redo
            redo(list, undoStack, redoStack);
        }

        reorderLines(list);
        deleteNewLines(list);
    }

    free(command);
    return 0;
}

int main()
{
    ListText *finalList, *list;

    list = initList();
    finalList = initList();
    undoStack = initStack();
    redoStack = initStack();

    in = fopen("editor.in", "r");
    out = fopen("editor.out", "w");
    int what = 1;

    while (1)
    {
        if (what == 1)
        {
            // mod inserare text
            // verific daca am ajuns la final de fisier
            // sau daca s-a inchis editorul

            if (!addText(list))
                break;
        }
        else
        {
            // mod inserare comenzi
            // verific daca am ajuns la final de fisier
            // sau daca s-a inchis editorul

            if (!doCommands(list, finalList))
                break;
        }

        what = 1 - what;
    }

    printList(finalList, out);

    deleteList(&list);
    deleteList(&finalList);
    deleteStack(undoStack);
    deleteStack(redoStack);

    free(list);
    free(finalList);
    free(undoStack);
    free(redoStack);

    fclose(in);
    fclose(out);
}

#include "editor.h"
#include "stack.h"

FILE *in, *out;
Stack *stack;

ListText *init()
{
    // initializez atat cele doua liste cat si stiva
    ListText *list = (ListText *)malloc(sizeof(ListText));
    list->head = NULL;
    list->cursor = NULL;
    list->tail = NULL;

    return list;
}

int addText(ListText *list)
{
    char chr;
    int ok = 0;
    while (fscanf(in, "%c", &chr))
    {
        if (chr != ':')
        {
            if (ok == 1)
                insertCharacter(list, ':');
            insertCharacter(list, chr);
            ok = 0;
        }
        else
        {
            if (ok == 1)
            {
                // se trece la modul de inserare comenzi
                fscanf(in, "%c", &chr);
                return 1;
            }
            ok = 1;
        }
    }

    if (ok == 1)
        insertCharacter(list, ':');

    return 0;
}

int doCommands(ListText *list, ListText *finalList)
{
    char *command;
    command = (char *)malloc(100 * sizeof(char));

    // !!! de bagat comenzile in stiva
    while (fgets(command, 100, in))
    {
        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if (strcmp("::i", command) == 0)
        {
            // se trece la modul inserare text
            return 1;
        }
        else if (strcmp("q", command) == 0)
        {
            // se inchide editorul
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
        }
        else if (command[0] == 'b')
        {
            //sterge caracterul de dinaintea cursorului
            backspace(list);
        }
        else if (command[0] == 'd')
        {
            // sterge un numar de caractere incepand cu pozitia curenta
            int num = getNum(command + 2);
            delete (list, num);
        }
    }

    return 0;
}

int main()
{
    ListText *finalList, *list;

    list = init();
    finalList = init();

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

    fclose(in);
    fclose(out);
}

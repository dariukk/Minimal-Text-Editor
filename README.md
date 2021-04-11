# Tema 1 SD - Daria-Mihaela Florea, 311CC

    Implementarea editorului a fost realizata pe parcursul a doua saptamani, insumand un total de peste 20 de ore de scris cod si facut debugging.

    Pentru realizarea editorului de text au fost implementate cinci fisiere sursa C (editor.c, commands.c, stack.c, undo.c, redo.c), respectiv un header (editor.h).


  # editor.c

    In editor.c sunt implementate trecerea de la modul inserare text la modul inserare comenzi pana la intalnirea comenzii q, respectiv implemntarile celor doua moduri. 
    Modul inserare text va adauga intr-o lista de caractere in pozitia cursorului (cu ajutorul functie insertCharacter) textul introdus de utilizator. Aceasta operatie este repetata pana la intalnirea comenzii ::i prin care se face trecerea la modul inserare comenzi. La finalul acestui mod operatia va fi adaugata in stiva de undo (undoStack).
    Modul inserare comenzi citeste pana la intalnirea comenzii ::i sau a comenzii q comenzile introduse de utilizator si apeleaza functiile necesare pentru efectuarea acestora, iar dupa apel adauga in stiva de undo (undoStack) operatia anterior realizata si apeleaza functiile reorderLines (dupa anumite comenzi este posibil ca numarul liniilor sa se schimbe) si deleteNewLines (textul final nu trebuie sa contina linii goale).


  #  commands.c

    In commands.c sunt implementate functiile corespunzatoare implementarii tuturor comenzilor, cu exceptia comenzilor undo si redo.
    
    Pentru comanda save apelez functia save, in care sterg elementele existente in lista finalList si copiez in aceasta elementele listei list.
    
    Pentru comanda goto line apelez functia gotoLine care muta cursorul pe linia
indicata si retine pozitia anterioara a cursorului daca comanda nu este apelata
in cadrul comenzii undo. In mod asemanator, pentru comanda goto character, 
functia gotoChar muta cursorul in pozitia indicata.

    Pentru comanda delete line apelez functia deleteLine. Aceasta trateaza trei
cazuri distincte: 
    1) prima linie este stearsa 
    2) ultima linie este stearsa
    3) o linie din interiorul textului este stearsa*/

    Pentru comanda backspace apelez functia cu acelasi nume. Aceasta trateaza 
doua cazuri:
    1) cursorul se afla pe ultima pozitie din text
    2) cursorul nu se afla pe ultima pozitie
    Analog se efectueaza si operatia delete, doar ca se va trata separat cazul
in care cursorul se afla la inceput de linie. Pentru ambele comenzi, adaug in
stiva de undo (undoStack) elementele ce au fost sterse.

    Pentru comanda delete word apelez functia deleteWord in care caut prima
aparitie a cuvantului introdus de utilizator pe care o voi sterge ulterior.
Adaug in stiva de undo (undoStack) cuvantul sters si pozitia la care se afla
inainte de efectuarea comenzii. Pentru comanda delete all apelez functia
deleteAllWords, in care aplic acelasi mecanism ca in functia deleteWord,
stergand toate aparitiile cuvantului dat, nu doar prima aparitie.

    Asemantor comenzilor delete word si delete all, pentru comenzile replace,
respectiv replace all voi cauta prima aparitie, respectiv ultima aparatie a
cuvantului vechi si, in plus fata de comenzile delete word si delete all, voi
insera noul cuvant.



   # stack.c 

    In stack.c sunt implementate operatiile necesare stivelor de undo, respectiv
de redo (undoStack, redoStack) : initStack (iniializeaza o stiva goala),
push (adauga un element in stiva), pop (elimina elementul din varful stivei),
top (returneaza valoarea elementului din varful stivei) , deleteStack (sterge
si elibereaza elementele dintr-o stiva).

   # undo.c & redo.C
    
    In undo.c, respectiv redo.c sunt implementate comenzile undo, respectiv
redo. Pentru a efectua comanda undo trebuie sa aplicam comanda inversa comenzii
anterioara, iar pentru comanda redo trebuie doar sa aplicam ultima comanda
careia i s-a aplicat comanda undo. Dupa efectuarea comenzii undo, adaug in stiva
de redo comanda efectuata, iar dupa efectuarea comenzii redo, adaug in stiva de
undo comanda efectuata. 

    #editor.h 

    In header sunt definitiile structurilor folosite si antetele functiilor.

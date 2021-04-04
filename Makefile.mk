all: editor.c stack.c commands.c undo.c redo.c editor.h

build: editor.c stack.c commands.c undo.c redo.c editor.h
	gcc -g -Wall -o editor editor.c stack.c commands.c undo.c redo.c

clean:
	rm -f editor

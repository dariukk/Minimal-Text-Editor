CC=gcc
CFLAGS=-I.
DEPS = editor.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

editor: editor.c 
	$(CC) -o editor editor.c stack.c commands.c undo.c

clean:
	rm -f editor

CC=gcc
CFLAGS=-I.
DEPS = editor.h stack.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: editor.c 
	$(CC) -o editor editor.c stack.c commands.c undo.c

clean:
	rm -f editor

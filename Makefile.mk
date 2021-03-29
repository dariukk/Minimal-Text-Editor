CC=gcc
CFLAGS=-I.
DEPS = editor.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

editor: editor.c 
	$(CC) -o hellomake editor.c 

clean:
	rm -f editor
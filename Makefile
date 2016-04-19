CC=gcc -g  -w
CFLAGS=-I inc

similar: src/main.c src/similar.c
	$(CC) $(CFLAGS) -o similar src/main.c src/similar.c 

clean: 
	rm -rf similar

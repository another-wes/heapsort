CC=gcc
heapsort: heapsort.c
	$(CC) -Wall -ansi -pedantic-errors -std=c99 heapsort.c -o heapsort.exe
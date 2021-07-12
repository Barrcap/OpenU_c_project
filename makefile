assember: main.o
	gcc -g -ansi -pedantic -Wall main.o -o assember
main.o: main.c main.h
	gcc -c -g -ansi -pedantic -Wall main.c -o main.o
assember: main.o fileCompiler.o
	gcc -g -ansi -pedantic -Wall main.o fileCompiler.o -o assember
main.o: main.c main.h fileCompiler.h
	gcc -c -g -ansi -pedantic -Wall main.c -o main.o
fileCompiler.o: fileCompiler.c fileCompiler.h
	gcc -c -g -ansi -pedantic -Wall fileCompiler.c -o fileCompiler.o
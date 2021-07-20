assember: main.o fileCompiler.o
	gcc -g -ansi -pedantic -Wall main.o fileCompiler.o data.o -o assember
main.o: main.c main.h fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall main.c -o main.o
fileCompiler.o: fileCompiler.c fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall fileCompiler.c -o fileCompiler.o
data.o: data.c data.h
	gcc -c -g -ansi -pedantic -Wall data.c -o data.o
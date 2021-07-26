assember: main.o fileCompiler.o data.o toBinary.o
	gcc -g -ansi -pedantic -Wall main.o fileCompiler.o data.o toBinary.o -o assember
main.o: main.c main.h fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall main.c -o main.o
fileCompiler.o: fileCompiler.c fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall fileCompiler.c -o fileCompiler.o
data.o: data.c data.h
	gcc -c -g -ansi -pedantic -Wall data.c -o data.o
toBinary.o: toBinary.c toBinary.h command_table.h fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall toBinary.c -o toBinary.o
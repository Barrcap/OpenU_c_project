assembler: main.o fileCompiler.o data.o toBinary.o commandTable.o
	gcc -g -ansi -pedantic -Wall main.o fileCompiler.o data.o toBinary.o commandTable.o -o assembler
main.o: main.c main.h fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall main.c -o main.o
fileCompiler.o: fileCompiler.c fileCompiler.h data.h commandTable.h
	gcc -c -g -ansi -pedantic -Wall fileCompiler.c -o fileCompiler.o
data.o: data.c data.h commandTable.h
	gcc -c -g -ansi -pedantic -Wall data.c -o data.o
toBinary.o: toBinary.c toBinary.h commandTable.h fileCompiler.h data.h
	gcc -c -g -ansi -pedantic -Wall toBinary.c -o toBinary.o
commandTable.o: commandTable.c commandTable.h
	gcc -c -g -ansi -pedantic -Wall commandTable.c -o commandTable.o
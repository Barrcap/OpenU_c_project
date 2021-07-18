#include <stdio.h>
#include "fileCompiler.h"

#define LINE_LENGTH 80

int fileCompiler(char *fileName)
{
	FILE *file;
	char line[LINE_LENGTH+1];
	int reachedEOF, errorCounter = 0;

	int testCounter = 1;

	file = fopen(fileName, "r");

	if (file == NULL)
	/*	failed to open file */
		return 1;

	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(file, line, &reachedEOF) == 0)
			errorCounter += CodingLineTake1(line);
		else
			errorCounter ++;

		printf("line number %i is: %s\n", testCounter, line);
		testCounter ++;
	}

	/*
	fseek(file, 0, SEEK_SET);
	lineCounter = 1;

	while (file != EOF)
	{
		errorCounter += readFileLine(file, line);
		
		errorCounter += CodingLineTake2(line);
	}
	*/




	fclose(file);

	return errorCounter;
}

int readFileLine(FILE *file, char *line, int *reachedEOF)
{
	int c, i = 0;

	while ((c=fgetc(file)) != EOF && c!='\n')
	{
		if (i >= LINE_LENGTH)
		{
			printf("Line too long!\n");
			return 1;
		}

		line[i] = c;
		i++;
	}

	if (c == EOF)
		*reachedEOF = 1;
	line[i] = 0;

	return 0;
}

int CodingLineTake1(char *line)
{
	return 0;
}
#include <stdio.h>
#include "fileCompiler.h"


int fileCompiler(char *fileName)
{
	FILE *file;
	char line[LINE_LENGTH+1];
	int reachedEOF, errorCounter = 0;

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
	}

	
	fseek(file, 0, SEEK_SET);
	
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(file, line, &reachedEOF) == 0)
			errorCounter += CodingLineTake2(line);
		else
			errorCounter ++;
	}
	




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

int CodingLineTake2(char *line)
{
	return 0;
}
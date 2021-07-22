#include <stdio.h>
#include <string.h>
#include "data.h"
#include "fileCompiler.h"



int fileCompiler(char *fileName)
{
	FILE *file;
	char line[LINE_LENGTH+1];
	int reachedEOF, errorCounter = 0;
	fileCodingStruct codingData;


	file = fopen(fileName, "r");

	if (file == NULL)
	/*	failed to open file */
		return 1;
	

	/* initialize file data's both basic values, and all 3 data tables */
	resetCounterParams(&codingData);
	strcpy(codingData.fileName, fileName);

	if (createTables(&codingData) != 0)
	{
		printf("Failed allocating memory for %s! aborting file compilation.\n", fileName);
	}

	/* First time going over source code */
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(file, line, &reachedEOF, &codingData) == 0)
			errorCounter += CodingLineTake1(line);
		else
		{
			printError("Line too long", &codingData);
			errorCounter ++;
		}
		codingData.sourceLine ++;
	}

	

	/* Second time going over source code */
	fseek(file, 0, SEEK_SET);
	resetCounterParams(&codingData);
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(file, line, &reachedEOF, &codingData) == 0)
			errorCounter += CodingLineTake2(line);
		codingData.sourceLine ++;
	}
	



	freeTables(&codingData);
	fclose(file);

	return errorCounter;
}

int readFileLine(FILE *file, char *line, int *reachedEOF, fileCodingStruct *codingData)
{
	int c, i = 0;

	while ((c=fgetc(file)) != EOF && c!='\n')
	{
		if (i >= LINE_LENGTH)
			return 1;

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

void printError(char *errorString, struct fileCodingStruct *codingData)
{
	/*printf("%s:%i: %s\n", codingData->fileName, codingData->sourceLine, errorString);*/
	printf(RED "%s:%i: " RESET, codingData->fileName, codingData->sourceLine);
	printf("%s\n", errorString);
}


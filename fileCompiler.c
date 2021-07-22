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

	/*#############################* /
	printf("Before initialization:\n");
	printf("ic: %i\tdc: %i\tSourceline: %i\n", codingData.ic, codingData.dc, codingData.sourceLine);
	/ *#############################*/
	

	/* initialize file data's both basic values, and all 3 data tables */
	resetCounterParams(&codingData);
	/*#############################* /
	printf("After initialization:\n");
	printf("ic: %i\tdc: %i\tSourceline: %i\n", codingData.ic, codingData.dc, codingData.sourceLine);
	/ *#############################*/
	/*codingData.ic = 100;
	codingData.dc = 0;
	codingData.sourceLine = 1;*/
	strcpy(codingData.fileName, fileName);

	if (createTables(codingData.iTable, codingData.dTable, codingData.symbolTable) != 0)
	{
		printf("Failed allocating memory for %s! aborting file compilation.\n", fileName);
	}

	reachedEOF = 0;
	while (!reachedEOF)
	{
		/*#############################* /
		printf("First loop:\t");
		printf("ic: %i\tdc: %i\tSourceline: %i\n", codingData.ic, codingData.dc, codingData.sourceLine);
		/ *#############################*/
		if (readFileLine(file, line, &reachedEOF) == 0)
			errorCounter += CodingLineTake1(line);
		else
			errorCounter ++;
		codingData.sourceLine ++;
	}

	
	fseek(file, 0, SEEK_SET);
	resetCounterParams(&codingData);
	/*#############################* /
	printf("After resetting:\n");
	printf("ic: %i\tdc: %i\tSourceline: %i\n", codingData.ic, codingData.dc, codingData.sourceLine);
	/ *#############################*/
	/*codingData.ic = 100;
	codingData.dc = 0;
	codingData.sourceLine = 1;*/
	reachedEOF = 0;
	while (!reachedEOF)
	{
		/*#############################* /
		printf("Second loop:\t");
		printf("ic: %i\tdc: %i\tSourceline: %i\n", codingData.ic, codingData.dc, codingData.sourceLine);
		/ *#############################*/
		if (readFileLine(file, line, &reachedEOF) == 0)
			errorCounter += CodingLineTake2(line);
		else
			errorCounter ++;
		codingData.sourceLine ++;
	}
	



	freeTables(codingData.iTable, codingData.dTable, codingData.symbolTable);
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


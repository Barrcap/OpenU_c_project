#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
			errorCounter += encodingLineTake1(line, &codingData);
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
			errorCounter += encodingLineTake2(line, &codingData);
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

int encodingLineTake1(char *line, struct fileCodingStruct *codingData)
{
	char lable[LABLE_SIZE];
	char command[COMMAND_SIZE];
	char operands[OPERANDS_SIZE];
	/* Todo - add lable and command lengh validation */

	/*
	sscanf(line, lable);
	if (lable[strlen(lable)-1] == ':');*/



	return 0;
}

int encodingLineTake2(char *line, struct fileCodingStruct *codingData)
{
	return 0;
}

void printError(char *errorString, struct fileCodingStruct *codingData)
{
	/*printf("%s:%i: %s\n", codingData->fileName, codingData->sourceLine, errorString);*/
	printf(RED "%s:%i: " RESET, codingData->fileName, codingData->sourceLine);
	printf("%s\n", errorString);
}


int seperateArguments(char *line, char *lable, char command, char *operands, struct fileCodingStruct *codingData)
{
	char c;
	int i, p;

	while (isspace(line[i]))
		/* skips white notes at the beginning */
		i++;

	p=i; /* p is now operand first character */

	while (!isspace(line[i]))
		i++;


	/*char firstOperand[LINE_LENGTH];
	char secondOperand[LINE_LENGTH];*/

	/*sscanf(line, "%s %s", firstOperand, secondOperand);

	if (firstOperand[strlen(firstOperand)-1] == ':')
	{
		/ * First string until white note is a lable * /
		if (strlen(firstOperand) <= LABLE_SIZE)
			strcpy(lable, firstOperand);
		else
		{
			printError("Lable is too long", codingData);
			return 1;
		}

		if (strlen(secondOperand) <= COMMAND_SIZE)
			strcpy(command, secondOperand);
		else
		{
			printError("Invalid command name", codingData);
			return 1;
		}
	}*/

	return 0;
}


/*##################################################################################*/
long int roiEncoding(char *command, char *operands) /*dummy function Roi's encoding function */
{
	return 0;
}
/*##################################################################################*/
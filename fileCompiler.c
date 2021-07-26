#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "data.h"
#include "fileCompiler.h"
#include "toBinary.h"




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
	codingData.sc = 0; /* symbol table counter */
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

	

	/* Second time going over source code *  /
	fseek(file, 0, SEEK_SET);
	resetCounterParams(&codingData);
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(file, line, &reachedEOF, &codingData) == 0)
			errorCounter += encodingLineTake2(line, &codingData);
		codingData.sourceLine ++;
	}*/
	



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
		{
			while ((c=fgetc(file)) != EOF && c!='\n');
			/* finish reading rest of the line*/
			if (c == EOF)
				*reachedEOF = 1;
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

int encodingLineTake1(char *line, struct fileCodingStruct *codingData)
{
	char lable[LABLE_SIZE] = {0};
	char command[COMMAND_SIZE] = {0};
	char operands[LINE_LENGTH] = {0};

	int returnVal;

	returnVal = seperateArguments(line, lable, command, operands, codingData);

	if (returnVal != 0)
	{
		if (returnVal == 1)
			return 1;
		else
			return 0;
	}

	/* now lable, command, and operands strings are seperated*/
	/* Todo - content validation for label */



	/* Todo - content validtation for command and operands  */

	/* Deal with encoding function */

	/*printError("NOT ERROR - Coding line:", codingData); ############################*/
	toBinary(operands, command, codingData);


	return 0;
}

int encodingLineTake2(char *line, struct fileCodingStruct *codingData)
{
	return 0;
}

void printError(char *errorString, struct fileCodingStruct *codingData)
{
	/*printf("%s:%i: %s\n", codingData->fileName, codingData->sourceLine, errorString);*/
	printf(BOLDWHITE "%s:%i: " RESET, codingData->fileName, codingData->sourceLine);
	printf(BOLDRED "%s\n" RESET, errorString);
}


int seperateArguments(char *line, char *lable, char *command, char *operands, struct fileCodingStruct *codingData)
{
	int start=0, end, reachedNULL;

	reachedNULL = operandPointers(line, &start, &end);

	if (line[start] == 0)
		/* line contains only white notes */
	{
		return 2;
	}

	if (line[start] == ';')
		/* comment line */
	{
		return 3;
	}

	if (line[end-1] == ':')
	{
		/* First argument is a label */
		if (end-start > LABLE_SIZE)
		{
			printError("label too long", codingData);
			return 1;
		}

		line[end] = 0;
		strcpy(lable, line+start);
		if (reachedNULL)
		{
			printError("missing command (reachedNULL)", codingData);
			return 1;
		}
		start = end+1;
		

		operandPointers(line, &start, &end);
		if (line[start] == 0)
		{
			printError("missing command (only white notes after label)", codingData);
			return 1;
		}
	}

	/* start and end indexes now wrapping the command start/end */

	if (end-start > COMMAND_SIZE)
	{
		printError("invalid command", codingData);
		return 1;
	}

	line[end] = 0;
	strcpy(command, line+start);
	start = end+1;

	if (start < LINE_LENGTH)
		/* if went out of line's array or start is pointing to the array's NULL, operands will remain NULL */
		strcpy(operands, line+start);

	return 0;
}

int operandPointers(char *line, int *start, int *end)
{/*	function gets a string with source code line as string, a starting and ending indexes in the string.
	start index will be moved to the first none white character.
	end index will be moved to first white character/NULL after start index. */

	while (isspace(line[*start]))
		(*start)++;

	*end = *start;
	while (line[*end]!=0 && !isspace(line[*end]))
		(*end)++;

	if (line[*end]==0)
		return 1;

	return 0;

}


/*##################################################################################*/
long int roiEncoding(char *command, char *operands) /*dummy function Roi's encoding function */
{
	return 0;
}
/*##################################################################################*/
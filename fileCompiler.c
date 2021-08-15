#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "data.h"
#include "fileCompiler.h"
#include "toBinary.h"
#include "commandTable.h"
#include "validation.h"
#include "debugFlags.h"




int fileCompiler(char *fileName)
{
	FILE *sourceFile;
	char line[LINE_LENGTH+1];
	char objectFileName[FILE_NAME_SIZE];
	int reachedEOF, errorCounter = 0;
	fileCodingStruct codingData;
	

	sourceFile = fopen(fileName, "r");

	if (sourceFile == NULL)
	{
	/*	failed to open source file */
		printf("Failed to open %s\n",fileName);
		return 1;
	}
	

	/* initialize file data's both basic values, and all 3 data tables */
	resetCounterParams(&codingData);
	codingData.symbolLinkHead = NULL;

	strcpy(codingData.fileName, fileName);


	/* First time going over source code */
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(sourceFile, line, &reachedEOF, &codingData) == 0)
			errorCounter += encodingLineTake1(line, &codingData);
		else
		{
			printError("Line too long", &codingData);
			errorCounter ++;
		}
		codingData.sourceLine ++;
	}


	if (errorCounter != 0)
	{
		if (SHOW_GENERAL) printf("Found errors in Take1, aborting compilation for %s\n", fileName); /* debug print */
		freeSymbolTable(&codingData);
		fclose(sourceFile);
		return errorCounter;
	}


	if (SHOW_GENERAL) printf("\nGreat Success!! Finished Take1! \n\n"); /* debug print */

	finalizeSymbolTable(&codingData);

	/* for debugging - using SHOW_SYMBOL/IC/DC macros */
	if (SHOW_FINAL_COUNTERS) printf(BOLDRED"$$$\t ICF:%i  DCF:%i \t$$$\n"RESET, codingData.icf, codingData.dcf);
	if (SHOW_SYMBOL_TABLE) printSymbolTable(fileName, &codingData);


	/* Setting up for second read of source code: */

	fseek(sourceFile, 0, SEEK_SET);

	strcpy(objectFileName, fileName);
	/* change .as to .ob */
	objectFileName[strlen(objectFileName)-2] = 'o';
	objectFileName[strlen(objectFileName)-1] = 'b';

	codingData.objectFile = fopen(objectFileName, "w");
	if (codingData.objectFile == NULL)
	{
		printf("Failed to open %s for writing\n",objectFileName);
		freeSymbolTable(&codingData);
		fclose(sourceFile);
		return 1;
	}

	/* print IC and DC to object file */
	fprintf(codingData.objectFile, "%i %i\n", codingData.icf-100, codingData.dcf);

	createDataImage(&codingData);
	resetCounterParams(&codingData);

	/* Second time going over source code */
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(sourceFile, line, &reachedEOF, &codingData) == 0)
			errorCounter += encodingLineTake2(line, &codingData);
		codingData.sourceLine ++;
	}
	

	if (SHOW_GENERAL) printf("\nGreat Success!! Finished Take2! \n\n"); /* debug print */
	/* for debugging - using SHOW_SYMBOL/IC/DC macros */
	if (SHOW_SYMBOL_TABLE) printSymbolTable(fileName, &codingData);
	
	fclose(sourceFile);

	dataImageToFile(&codingData);
	fclose(codingData.objectFile);

	if (errorCounter == 0)
		errorCounter = createSymbolTableFiles(&codingData);


	freeDataImage(&codingData);
	freeSymbolTable(&codingData);
	if (errorCounter != 0)
		remove(objectFileName);
	

	return errorCounter;
}

int encodingLineTake1(char *line, struct fileCodingStruct *codingData)
{	/*
	retuns 0 on success, 1 on compiling error */
	char lable[LABEL_SIZE] = {0};
	char command[COMMAND_SIZE] = {0};
	char operands[LINE_LENGTH] = {0};

	int returnVal;

	returnVal = seperateArguments(line, lable, command, operands, codingData);

	if (returnVal == 1) /* error detected */
		return 1;
	else if (returnVal != 0)
		return 0; /* blank or comment line */

	/* now lable, command, and operands strings are seperated*/

	/* for debugging - using SHOW_LABLE/COMMAND/OPERANDS macros */
	if (SHOW_TAKE == 1 || SHOW_TAKE == 3) printTake(lable, command, operands, codingData);
	
	if (analyzeCommand(command, codingData))
	{
		printError("illegal command", codingData);
		return 1;
	}

	/* VALIDATE_OPERANDS is debugging flag to give us option to turn off validation */
	if (VALIDATE_OPERANDS && validateOperands(operands, codingData->validationCase, codingData))
		return 1;

	lable[strlen(lable)-1] = 0; /* removing ':' at end of lable */


	if (strcmp(lable,""))
	{	/* lable was defined */
		
		/* VALIDATE_LABLE is debugging flag to give us option to turn off validation */
		if (VALIDATE_LABLE && validateLabel(lable, codingData))
			return 1;

		if ((strcmp(".extern",command) == 0))
			printWarning("Ignoring label definition before .extern command", codingData);
		else
		{
			/* pushing lable to symbol table */
			if (pushLable(lable, codingData->imageType, INTERN, codingData))
				return 1;
		}
	}

	if ((strcmp(".extern",command) == 0))
	{	/* lable was declared as extern */
		if (pushLable(operands, codingData->imageType, EXTERN, codingData))
			return 1;
	}


	/* for debugging - using SHOW_IC/DC macros */
	if ((SHOW_TAKE == 1 || SHOW_TAKE == 3) && (SHOW_IC || SHOW_DC)) printCountersBefore(codingData);

	advanceImageCounter(command, operands, codingData);

	/* for debugging - using SHOW_IC/DC macros */
	if ((SHOW_TAKE == 1 || SHOW_TAKE == 3) && (SHOW_IC || SHOW_DC)) printCountersAfter(codingData);


	return 0;
}

int encodingLineTake2(char *line, struct fileCodingStruct *codingData)
{
	char lable[LABEL_SIZE] = {0};
	char command[COMMAND_SIZE] = {0};
	char operands[LINE_LENGTH] = {0};

	int returnVal;


	returnVal = seperateArguments(line, lable, command, operands, codingData);
	if (returnVal == 1) /* error detected */
		return 1;
	else if (returnVal != 0)
		return 0; /* blank or comment line */

	/* now lable, command, and operands strings are seperated*/
	
	/* for debugging - using SHOW_LABLE/COMMAND/OPERANDS macros */
	if (SHOW_TAKE == 2 || SHOW_TAKE == 3) printTake(lable, command, operands, codingData);

	if (analyzeCommand(command, codingData))
	{
		printError("illegal command", codingData);
		return 1;
	}

	if ((strcmp(".entry",command) == 0))
	{	/* lable was declared as extern */
		if (pushLable(operands, codingData->imageType, ENTRY, codingData))
			return 1;
	}

	/* Deal with encoding functions */

	if (codingData->imageType == CODE_IMAGE)
		toBinary(command, operands, codingData);

	if (codingData->imageType == DATA_IMAGE)
	{
		/* for debugging - printing data images's index */
		if (SHOW_DATA_ARR_I) printf("\tdata image index: %i->", codingData->dataImage->currIndex);

		if (strcmp(".asciz",command) == 0)
		{
			pushDataStr(operands, codingData);
		}
		else /* command is .db/.dh/.dw */
		{
			pushDataInt(operands, countOperands(operands), codingData);
		}

		/* for debugging - printing data images's index */
		if (SHOW_DATA_ARR_I) printf("%i\n", codingData->dataImage->currIndex);
	}
	
	/* for debugging - using SHOW_IC/DC macros */
	if ((SHOW_TAKE == 2 || SHOW_TAKE == 3) && (SHOW_IC || SHOW_DC)) printCountersBefore(codingData);

	advanceImageCounter(command, operands, codingData);

	/* for debugging - using SHOW_IC/DC macros */
	if ((SHOW_TAKE == 2 || SHOW_TAKE == 3) && (SHOW_IC || SHOW_DC)) printCountersAfter(codingData);

	return 0;
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

int seperateArguments(char *line, char *lable, char *command, char *operands, struct fileCodingStruct *codingData)
{
	int start=0, end, reachedNULL, lastCharIndex;

	if (SHOW_LINE) printf("Parsing line: '%s'\n", line);

	lastCharIndex = strlen(line)-1;

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
		if (end-start > LABEL_SIZE)
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
		

		reachedNULL = operandPointers(line, &start, &end);
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
	if (reachedNULL) /* Nothings to parse after the command */
		return 0;

	start = end+1;

	while (isspace(line[start]))
		start++;

	end = lastCharIndex;
	while (isspace(line[end]))
	{
		line[end] = 0;
		end --;
	}
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

int countOperands(char *operands)
{
	int i=0, commaCounter = 0;

	while (operands[i] != 0)
	{
		if (operands[i] == ',')
			commaCounter ++;

		i++;
	}

	return commaCounter+1;
}

int getStringLenght(char *operands)
{
	/*recieves string in the format "string"
	lenght take into considaration place for NULL, and ignores quatation marks */
	return strlen(operands)-1;
}

void advanceImageCounter(char *command, char *operands, fileCodingStruct *codingData)
{
	if (codingData->imageType == CODE_IMAGE)
		codingData->ic += 4;

	if (codingData->imageType == DATA_IMAGE)
	{
		if (strcmp(".asciz",command) == 0)
			codingData->dc += codingData->commandImageBytes * getStringLenght(operands);
		else
			codingData->dc += codingData->commandImageBytes * countOperands(operands);
	}
}

int createSymbolTableFiles(struct fileCodingStruct *codingData)
{
	char entFileName[FILE_NAME_SIZE];
	char extFileName[FILE_NAME_SIZE];

	FILE *entFile, *extFile;
	symbolLink *currLink;

	int lastCharIndex;

	strcpy(entFileName, codingData->fileName);
	/* change .as to .ent */
	lastCharIndex = strlen(entFileName)-1;
	entFileName[lastCharIndex-1]	= 'e';
	entFileName[lastCharIndex]		= 'n';
	entFileName[lastCharIndex+1]	= 't';
	entFileName[lastCharIndex+2]	=  0;

	strcpy(extFileName, entFileName);
	/* change .ent to .ext */
	extFileName[lastCharIndex]		= 'x';

	/* now entFileName = "FILENAME.ent" and extFileName = "FILENAME.ext" */


	entFile = fopen(entFileName, "w");
	if (entFile == NULL)
	{
		printf("Failed to open %s for writing\n",entFileName);
		return 1;
	}

	extFile = fopen(extFileName, "w");
	if (extFile == NULL)
	{
		printf("Failed to open %s for writing\n",extFileName);
		fclose(entFile);
		return 1;
	}


	currLink = codingData->symbolLinkHead;
	while (currLink)
	{
		if (currLink->visibility == ENTRY)
			fprintf(entFile, "%s %04i\n", currLink->name, currLink->adress);
		if (currLink->visibility == EXTERN)
			fprintf(extFile, "%s %04i\n", currLink->name, currLink->adress);

		currLink = currLink->next;
	}


	fclose(entFile);
	fclose(extFile);
	return 0;

}

void printSymbolTable(char *fileName, fileCodingStruct *codingData)
{
	symbolLink *currLink;

	printf(BOLDYELLOW"$$$ %s: SybmbolTable after Take1: $$$\n"RESET, fileName);
	currLink = codingData->symbolLinkHead;
	while (currLink)
	{
		printf("name: '%s'\t", currLink->name);
		printf("adress: '%i'\t", currLink->adress);
		printf("Attributes: ");
		if (currLink->placing == CODE_IMAGE) printf("code ");
		if (currLink->placing == DATA_IMAGE) printf("data ");
		if (currLink->visibility == ENTRY) printf("entry");
		if (currLink->visibility == EXTERN) printf("external");

		printf("\n");

		currLink = currLink->next;
	}
}

void printTake(char *lable, char *command, char *operands, fileCodingStruct *codingData)
{
	printf(BOLDYELLOW"line %i:"RESET, codingData->sourceLine);

	if (SHOW_LABLE)
		printf("\tlable:"BOLDWHITE"'%s'"RESET, lable);
	if (SHOW_COMMAND)
		printf("\tcommand:"BOLDWHITE"'%s'"RESET, command);
	if(SHOW_OPERANDS)
		printf("\toperands:"BOLDWHITE"'%s'"RESET, operands);

	printf("\n");
}

void printCountersBefore(fileCodingStruct *codingData)
{
	if (SHOW_IC && codingData->imageType == CODE_IMAGE)
		printf(BOLDRED"\tic: %i->", codingData->ic);
	if (SHOW_DC && codingData->imageType == DATA_IMAGE)
		printf(BOLDRED"\tdc:%i->", codingData->dc);
}

void printCountersAfter(fileCodingStruct *codingData)
{
	if (SHOW_IC && codingData->imageType == CODE_IMAGE)
		printf("%i\n"RESET, codingData->ic);
	if (SHOW_DC && codingData->imageType == DATA_IMAGE)
		printf("%i\n"RESET, codingData->dc);
}
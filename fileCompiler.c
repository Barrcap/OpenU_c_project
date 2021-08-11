#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "data.h"
#include "fileCompiler.h"
#include "toBinary.h"
#include "commandTable.h"
#include "validation.h"




int fileCompiler(char *fileName)
{
	FILE *sourceFile;
	char line[LINE_LENGTH+1];
	int reachedEOF, errorCounter = 0;
	fileCodingStruct codingData;
	

	symbolLink *currLink; /*##############################*/

	sourceFile = fopen(fileName, "r");

	if (sourceFile == NULL)
	/*	failed to open source file */
		return 1;
	

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


	printf("\nGreat Success!! Finished Take1! \n\n"); /* #################################### */

	finalizeSymbolTable(&codingData);

	{	/* for debugging - using SHOW_SYMBOL/IC/DC macros */

		if (SHOW_FINAL_COUNTERS)
			printf(BOLDRED"$$$\t ICF:%i  DCF:%i \t$$$\n"RESET, codingData.icf, codingData.dcf);


		if (SHOW_SYMBOL_TABLE)
		{
			printf(BOLDYELLOW"$$$ %s: SybmbolTable after Take1: $$$\n"RESET, fileName); /* #################################### */
			currLink = codingData.symbolLinkHead;
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
	}

	/* Second time going over source code */

	createDataImage(&codingData);
	fseek(sourceFile, 0, SEEK_SET);
	resetCounterParams(&codingData);
	reachedEOF = 0;
	while (!reachedEOF)
	{
		if (readFileLine(sourceFile, line, &reachedEOF, &codingData) == 0)
			errorCounter += encodingLineTake2(line, &codingData);
		codingData.sourceLine ++;
	}
	

	printf("\nGreat Success!! Finished Take2! \n\n"); /* #################################### */
	{	/* for debugging - using SHOW_SYMBOL/IC/DC macros */

		if (SHOW_SYMBOL_TABLE)
		{
			printf(BOLDYELLOW"$$$ %s: SybmbolTable after Take2: $$$\n"RESET, fileName); /* #################################### */
			currLink = codingData.symbolLinkHead;
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
	}


	freeDataImage(&codingData);
	freeSymbolTable(&codingData);
	fclose(sourceFile);

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
{	/*
	retuns 0 on success, 1 on compiling error */
	char lable[LABEL_SIZE] = {0};
	char command[COMMAND_SIZE] = {0};
	char operands[LINE_LENGTH] = {0};

	int returnVal, imageType, commandImageBytes;

	/*long int code=0; / *#######################################*/

	returnVal = seperateArguments(line, lable, command, operands, codingData);
	if (returnVal != 0)
	{
		if (returnVal == 1) /* error detected */
			return 1;
		else
			return 0; /* blank or comment line */
	}

	/* now lable, command, and operands strings are seperated*/


	{	/* for debugging - using SHOW_LABLE/COMMAND/OPERANDS macros */
		if (SHOW_TAKE == 1)
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
	}
	

	if (analyzeCommand(command, &imageType, &commandImageBytes, codingData))
	{
		printError("illegal command", codingData);
		return 1;
	}



	/* Todo - content validtation for operands  */

	/* Todo - content validation for lable */

	lable[strlen(lable)-1] = 0; /* removing ':' at end of lable */


	if (strcmp(lable,""))
	{	/* lable was defined */
		
		if (validateLabel(lable, codingData))
		return 1;

		if (pushLable(lable, imageType, INTERN, codingData))
			return 1;
	}

	if ((strcmp(".extern",command) == 0))
	{
		if (pushLable(operands, imageType, EXTERN, codingData))
			return 1;
	}







	{	/* for debugging - using SHOW_IC/DC macros */
		if ((SHOW_TAKE == 1) && (SHOW_IC || SHOW_DC))
		{
			if (SHOW_IC && imageType == CODE_IMAGE)
				printf(BOLDRED"\tic: %i->", codingData->ic); /* #####################################3##################### */
			if (SHOW_DC && imageType == DATA_IMAGE)
				printf(BOLDRED"\tdc:%i->", codingData->dc); /* #####################################3##################### */
		}
	}


	if (imageType == CODE_IMAGE)
	{
		codingData->ic += 4;
		
	}

	if (imageType == DATA_IMAGE)
	{
		if (strcmp(".asciz",command) == 0)
			codingData->dc += commandImageBytes * getStringLenght(operands);

		else /*if (strcmp(".entry",command)*strcmp(".extern",command) != 0)*/
			codingData->dc += commandImageBytes * countOperands(operands);
		
	}

	{	/* for debugging - using SHOW_IC/DC macros */
		if ((SHOW_TAKE == 1) && (SHOW_IC || SHOW_DC))
		{
			if (SHOW_IC && imageType == CODE_IMAGE)
				printf("%i\n"RESET, codingData->ic); /* #####################################3##################### */
			if (SHOW_DC && imageType == DATA_IMAGE)
				printf("%i\n"RESET, codingData->dc); /* #####################################3##################### */
		}
	}




	return 0;
}

int encodingLineTake2(char *line, struct fileCodingStruct *codingData)
{
	char lable[LABEL_SIZE] = {0};
	char command[COMMAND_SIZE] = {0};
	char operands[LINE_LENGTH] = {0};

	int returnVal, imageType, commandImageBytes;




	returnVal = seperateArguments(line, lable, command, operands, codingData);
	if (returnVal != 0)
	{
		if (returnVal == 1) /* error detected */
			return 1;
		else
			return 0; /* blank or comment line */
	}
	/* now lable, command, and operands strings are seperated*/

	{	/* for debugging - using SHOW_LABLE/COMMAND/OPERANDS macros */
		if (SHOW_TAKE == 2)
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
	}

	if (analyzeCommand(command, &imageType, &commandImageBytes, codingData))
	{
		printError("illegal command", codingData);
		return 1;
	}

	if ((strcmp(".entry",command) == 0))
	{
		if (pushLable(operands, imageType, ENTRY, codingData))
			return 1;
	}

	/* Deal with encoding function */

	if (imageType == CODE_IMAGE)
	{
		if (SHOW_ENCODING)
		{
			/*printError("\033[1m\033[33mNOT ERROR - Coding line:\033[0m", codingData);
			printf("lable: '%s'\tcommand: '%s'\toperands:'%s'\n", lable, command, operands);*/
			toBinary(command, operands, codingData);
		}
	}

	if (imageType == DATA_IMAGE)
	{
		if (SHOW_ENCODING)
		{
			printf("Theoratically coding command '%s' with operands '%s'\n", command, operands);
			/*pushDataCommand(command, operands, codingData);*/
		}
	}


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
	operandPointers(line, &start, &end);
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


/*##################################################################################*/
long int roiEncoding(char *command, char *operands) /*dummy function Roi's encoding function */
{
	return 0;
}
/*##################################################################################*/
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "data.h"
#include "commandTable.h"
#include "toBinary.h"
#include "debugFlags.h"





void freeSymbolTable(fileCodingStruct *codingData)
{
	symbolLink *curr, *next;
	curr = codingData->symbolLinkHead;

	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
}

void createDataImage(fileCodingStruct *codingData)
{

	codingData->dataImage = (dataImageStruct*) calloc(1,sizeof(dataImageStruct));
	if (codingData->dataImage == NULL)
		exit(EXIT_FAILURE); /* failed allocating memory for dataImage struct */

	codingData->dataImage->size = codingData->dcf;
	codingData->dataImage->currIndex = 0;

	codingData->dataImage->array = (char*) calloc(codingData->dataImage->size,sizeof(char));
	if (codingData->dataImage->array == NULL)
		exit(EXIT_FAILURE); /* failed allocating memory for dataImage's array */
}

void freeDataImage(fileCodingStruct *codingData)
{
	free(codingData->dataImage->array);
	free(codingData->dataImage);
}

void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 0;
	codingData->sourceLine = 1;
}


int getIC(fileCodingStruct *codingData)
{
	return codingData->ic;
}

int analyzeCommand(char *commandName, fileCodingStruct *codingData)
{
	int i;

	for (i=0; i<COMMAND_NUM; i++)
		if (strcmp(commandName,lines[i].command) == 0)
		{
			codingData->imageType = CODE_IMAGE;
			codingData->commandImageBytes = 4;
			codingData->validationCase = lines[i].validationCase;
			return 0;
		}

	for (i=0; i<DATA_COMMANDS; i++)
		if (strcmp(commandName,dataCommands[i].name) == 0)
		{
			/**imageType = DATA_IMAGE;*/
			/* check if .entry or .extern */
			if (strcmp(".entry",dataCommands[i].name)*strcmp(".extern",dataCommands[i].name) == 0)
				codingData->imageType = NONE;
			else
				codingData->imageType = DATA_IMAGE;

			codingData->commandImageBytes = dataCommands[i].bytes;
			codingData->validationCase = dataCommands[i].validationCase;
			return 0;
		}

	codingData->validationCase = -1;
	return 1;
}

int pushLable(char *lable, int placing, int visibility, fileCodingStruct *codingData)
{/*	
	returns 0 on success, 1 if found error
	INTERN and EXTERN labels are pushed at Take1
	ENTRY label is pushed at Take2 */


	symbolLink *currLink, *lastLink;

	currLink = codingData->symbolLinkHead;
	lastLink = currLink;

	/* checking if lable was already defined: */
	while (currLink) 
	{
		if (strcmp(lable,currLink->name) == 0)
		{
			/* lable was already defined */
			switch (currLink->visibility)
			{
				case INTERN: /* visibility of existing label */

					if (visibility == ENTRY) /* visibility of new label */
					{
						currLink->visibility = ENTRY;
						return 0;
					}

					printError("Label already defined", codingData);
					return 1;
					break;

				case EXTERN: /* visibility of existing label */

					if (visibility == INTERN) /* visibility of new label */
					{
						printError("Label already defined as extern", codingData);
						return 1;
					}
					if (visibility == EXTERN) /* visibility of new label */
						return 0;
					break;

				case ENTRY: /* visibility of existing label */

					if (visibility == ENTRY) /* visibility of new label */
						return 0;
					break;

			}/* switch case ends*/
		}

		lastLink = currLink;
		currLink = lastLink->next;
	}
	/* lable wasn't defined yet, lastLink is pointing at last link, if exists */
	
	if (visibility == ENTRY) /* visibility of new label */
	{
		printError("entry command for undefined lable", codingData);
		return 1;
	}


	if (codingData->symbolLinkHead == NULL)
	{	/* definging now the first lable in symbolLink list */
		codingData->symbolLinkHead = (symbolLink*) calloc(1,sizeof(symbolLink));
		currLink = codingData->symbolLinkHead;
	}
	else
	{
		lastLink->next = (symbolLink*) calloc(1,sizeof(symbolLink));
		currLink = lastLink->next;
	}

	/* currLink is now pointing on new created link */

	if (currLink == NULL)
		exit(EXIT_FAILURE); /* failed allocating memory for new symbol link */

	strcpy(currLink->name, lable);

	if (visibility == INTERN)
	{
		if (placing == CODE_IMAGE)
			currLink->adress = codingData->ic;

		if (placing == DATA_IMAGE)
			currLink->adress = codingData->dc;

		currLink->placing = placing;
	}

	if (visibility == EXTERN)
	{
		currLink->adress = 0;
		currLink->placing = NONE;
	}
	
	currLink->visibility = visibility;

	return 0;
}


int getLabelAdress(char *lableName, fileCodingStruct *codingData)
{	/* returns lable's adress, if lable doesn't exist returns -1 */
	symbolLink *currLink;

	currLink = codingData->symbolLinkHead;


	while (currLink)
	{
		if (strcmp(lableName,currLink->name) == 0)
		{
			if (currLink->visibility == EXTERN)
				return 0;
			else
				return currLink->adress;
		}


		currLink = currLink->next;
	}
	return -1;
}

void finalizeSymbolTable(fileCodingStruct *codingData)
{	/* icf is next available adress */
	symbolLink *currLink;

	currLink = codingData->symbolLinkHead;
	codingData->icf = codingData->ic;
	codingData->dcf = codingData->dc;
	while (currLink)
	{
		if (currLink->placing == DATA_IMAGE)
			currLink->adress += codingData->icf;

		currLink = currLink->next;
	}
}

void pushCode(long int code, fileCodingStruct *codingData)
{
	int i;
	unsigned char mask;
	long int codeForFile = code;

	fprintf(codingData->objectFile,"%04i ",codingData->ic);

	for (i=0; i<4; i++)
	{
		mask = 0;
		mask |= codeForFile;
		fprintf(codingData->objectFile, "%02X", mask);
		if (i<3)
			fprintf(codingData->objectFile, " ");
		codeForFile >>= 8;
	}
	fprintf(codingData->objectFile, "\n");

	/* for debugging purpose */
	if(SHOW_ENCODING) binPrint(code);

	/* for debugging purpose */
	if (FILE_BINARY_PRINT)
	{
	    for (i = 1 << 31; i > 0; i = i / 2)
	        (code & i) ? fprintf(codingData->objectFile,"1") : fprintf(codingData->objectFile,"0");
	    fprintf(codingData->objectFile,"\n\n");
	}

}

void pushDataInt(char *operands, int argumentsAmount, fileCodingStruct *codingData)
{
	long int val;
	unsigned char mask;
	int i, j;
	char operandsToRuin[LINE_LENGTH];

	strcpy(operandsToRuin,operands);

	val = atol( strtok(operandsToRuin,",") );

	for (i=0; i<argumentsAmount; i++)
	{
		for (j=0; j<codingData->commandImageBytes; j++)
		{
			mask = 0 | val;
			if (codingData->dataImage->currIndex >= codingData->dataImage->size)
			{
				printError("BUG!!!! went out of Data Image array!!!!! (INT)", codingData);
				return;
			}
			codingData->dataImage->array[codingData->dataImage->currIndex] = mask;
			val >>= 8;
			codingData->dataImage->currIndex ++;
		}

		if (i+1<argumentsAmount)
			val = atol( strtok(NULL,",") );
	}

}

void pushDataStr(char *operands, fileCodingStruct *codingData)
{
	int i;

	for (i=1; i<strlen(operands)-1; i++)
	{
		if (codingData->dataImage->currIndex >= codingData->dataImage->size)
		{
			printError("BUG!!!! went out of Data Image array!!!!! (STR)", codingData);
			return;
		}
		codingData->dataImage->array[codingData->dataImage->currIndex] = operands[i];
		codingData->dataImage->currIndex ++;
	}
	codingData->dataImage->array[codingData->dataImage->currIndex] = 0;
	codingData->dataImage->currIndex ++;
}

void dataImageToFile(fileCodingStruct *codingData)
{
	int i=0;
	char mask;


	while (i<codingData->dataImage->size)
	{
		if (i%4 == 0)
			fprintf(codingData->objectFile,"%04i",codingData->icf + i);

		mask = codingData->dataImage->array[i];
		fprintf(codingData->objectFile, " %02X", (mask & 0xFF));

		if (i%4 == 3)
			fprintf(codingData->objectFile, "\n");
		/*else
			fprintf(codingData->objectFile, " ");*/
		i++;
	}

}

void pushExtUsage(char *label, fileCodingStruct *codingData)
{
	fprintf(codingData->extFile, "%s %04i\n", label, codingData->ic);
}

void printError(char *errorString, struct fileCodingStruct *codingData)
{
	printf(BOLDWHITE "%s:%i: " RESET, codingData->fileName, codingData->sourceLine);
	printf(BOLDRED "%s\n" RESET, errorString);
}

void printWarning(char *errorString, struct fileCodingStruct *codingData)
{
	printf(BOLDWHITE "%s:%i: " RESET, codingData->fileName, codingData->sourceLine);
	printf(BOLDYELLOW "WARNING: %s\n" RESET, errorString);
}
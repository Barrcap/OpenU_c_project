#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h> /* can be removed with fileCompiler.h? ######################*/

#include "data.h"
#include "fileCompiler.h" /* move printError here? ############################## */
#include "commandTable.h"
#include "toBinary.h" /* to be removed? ############################  */




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

void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 0;
	codingData->sourceLine = 1;
}

void advanceImageCounter(int imageType, fileCodingStruct *codingData)
{
	switch (imageType)
	{
		case CODE_IMAGE:

			break;

		case DATA_IMAGE:

			break;
	}
}

int getIC(fileCodingStruct *codingData)
{
	return codingData->ic;
}

int analyzeCommand(char *commandName, int *imageType, int *commandImageBytes, fileCodingStruct *codingData)
{
	int i;

	for (i=0; i<COMMAND_NUM; i++)
		if (strcmp(commandName,lines[i].command) == 0)
		{
			*imageType = CODE_IMAGE;
			*commandImageBytes = 4;
			return 0;
		}

	for (i=0; i<DATA_COMMANDS; i++)
		if (strcmp(commandName,dataCommands[i].name) == 0)
		{
			*imageType = DATA_IMAGE;
			*commandImageBytes = dataCommands[i].bytes;
			return 0;
		}

	return 1;
}

int pushLable(char *lable, int placing, int visibility, fileCodingStruct *codingData)
{/*	
	returns 0 on success, 1 if found error */
	/*char *errorString[LINE_LENGTH];
	char *sourceLineString[20];*/

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
				case INTERN:
					printError("Label already defined", codingData);
					return 1;
					break;

				case EXTERN:
					if (visibility == INTERN)
					{
						printError("Label already defined as extern", codingData);
						return 1;
					}
					if (visibility == EXTERN)
						return 0;
					break;

			}/* switch case ends*/
		}

		lastLink = currLink;
		currLink = lastLink->next;
	}
	/* lable wasn't defined yet, lastLink is pointing at last link, if exists */
	

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
		currLink->adress = 0;
	
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
			return currLink->adress;


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
	binPrint(code);
	
	/* Temprorary until start using tables: ############################### * /
	codingData->code = code;*/
}
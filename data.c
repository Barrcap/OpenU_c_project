#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h> /* can be removed with fileCompiler.h? ######################*/

#include "data.h"
#include "fileCompiler.h" /* move printError here? ############################## */

int createTables(fileCodingStruct *codingData)
{

	codingData->iTable = (dataCell*) calloc(TABLE_BUFFER, sizeof(dataCell));
	codingData->iTableSize = TABLE_BUFFER;
	codingData->iCurrIndex = 0;
	if (codingData->iTable == NULL)
		return 1;

	codingData->dTable = (dataCell*) calloc(TABLE_BUFFER, sizeof(dataCell));
	codingData->dTableSize = TABLE_BUFFER;
	codingData->dCurrIndex = 0;
	if (codingData->dTable == NULL)
		return 1;

	codingData->sTable = (symbolCell*) calloc(TABLE_BUFFER, sizeof(symbolCell));
	codingData->sTableSize = TABLE_BUFFER;
	codingData->sCurrIndex = 0;
	if (codingData->sTable == NULL)
		return 1;

	return 0;
}


void freeTables(fileCodingStruct *codingData)
{

	free(codingData->iTable);
	free(codingData->dTable);
	free(codingData->sTable);
	
}

int expandTableIfNeeded(int tableType, fileCodingStruct *codingData)
{	/* function gets table type with general struct and expand relevant table if needed.
	return 0 is succeeded, 1 if failed. */
	switch (tableType)
	{
		case I_TABLE:

			if (codingData->iCurrIndex < codingData->iTableSize)
				return 0; /* no need to expand table */

			codingData->iTableSize += TABLE_BUFFER;
			codingData->iTable = (dataCell*) realloc(codingData->iTable, codingData->iTableSize);
			if (codingData->iTable == NULL)
				return 1; /* failed to allocate more memory */

			break;

		case D_TABLE:

			if (codingData->dCurrIndex < codingData->dTableSize)
				return 0; /* no need to expand table */

			codingData->dTableSize += TABLE_BUFFER;
			codingData->dTable = (dataCell*) realloc(codingData->dTable, codingData->dTableSize);
			if (codingData->dTable == NULL)
				return 1; /* failed to allocate more memory */

			break;

		case S_TABLE:

			if (codingData->sCurrIndex < codingData->sTableSize)
				return 0; /* no need to expand table */

			codingData->sTableSize += TABLE_BUFFER;
			codingData->sTable = (symbolCell*) realloc(codingData->sTable, codingData->sTableSize);
			if (codingData->sTable == NULL)
				return 1; /* failed to allocate more memory */

			break;
	}

	return 0;
}


void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 4;
	codingData->sourceLine = 1;
}

int getLabelAdress(char *lableName, fileCodingStruct *codingData)
{
	return 0;
}

int getIC(fileCodingStruct *codingData)
{
	return 0;
}

int pushCode(long int code, fileCodingStruct *codingData)
{
	if (expandTableIfNeeded(I_TABLE, codingData))
		return -1; /* failed expanding table */

	codingData->iTable[codingData->iCurrIndex].adress = codingData->ic;
	codingData->iCurrIndex ++;
	codingData->ic += 4;


	return 0;

	/* Temprorary until start using tables: ############################### * /
	codingData->code = code;*/
}

int pushLable(char *label, int placing, fileCodingStruct *codingData)
{/*	
	returns 0 on success, 1 if found error, -1 if failed to realloc symbol table size */
	int i=0;
	/*char *errorString[LINE_LENGTH];
	char *sourceLineString[20];*/

	/* checking if label was already defined: */
	for (i=0; i<codingData->sCurrIndex; i++) 
	{
		if (strcmp(label,codingData->sTable[i].name) == 0)
		{
			/* label was already defined, so visibility value was defined as well */
			switch (codingData->sTable[i].visibility)
			{
				case INTERN:
					printError("Label already defined", codingData);
					return 1;
					break;

				case EXTERN:
					printError("Label already defined as extern", codingData);
					return 1;
					break;

				case ENTRY:
					if (placing == CODE_IMAGE)
						codingData->sTable[i].adress = codingData->ic;

					if (placing == DATA_IMAGE)
						codingData->sTable[i].adress = codingData->dc;
					return 0;
					break;
			}/* switch case ends*/
		}
	}
	/* looking for label loop ended.
	i=sCurrIndex, pointing to next available spot in the table */

	if (expandTableIfNeeded(S_TABLE, codingData))
		return -1; /* failed expanding table */

	strcpy(codingData->sTable[i].name, label);
	if (placing == CODE_IMAGE)
		codingData->sTable[i].adress = codingData->ic;

	if (placing == DATA_IMAGE)
		codingData->sTable[i].adress = codingData->dc;

	codingData->sTable[i].placing = placing;
	codingData->sTable[i].visibility = INTERN;

	return 0;
}

void finalizeSymbolTable(fileCodingStruct *codingData)
{
	int i=0;

	for (i=0; i<codingData->sCurrIndex; i++)
	{
		if (codingData->sTable[i].placing == DATA_IMAGE)
			codingData->sTable[i].adress += codingData->icf;
	}
}
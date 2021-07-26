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
	if (codingData->iTable == NULL)
		return 1;

	codingData->dTable = (dataCell*) calloc(TABLE_BUFFER, sizeof(dataCell));
	codingData->dTableSize = TABLE_BUFFER;
	if (codingData->dTable == NULL)
		return 1;

	codingData->sTable = (symbolCell*) calloc(TABLE_BUFFER, sizeof(symbolCell));
	codingData->sTableSize = TABLE_BUFFER;
	codingData->sc = 0;
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

int expandTable(int tableType, fileCodingStruct *codingData)
{	/* function gets table type with general struct and expand relevant table.
	return 0 is succeeded, 1 if failed. */
	switch (tableType)
	{
		case I_TABLE:
			codingData->iTableSize += TABLE_BUFFER;
			codingData->iTable = (dataCell*) realloc(codingData->iTable, codingData->iTableSize);
			if (codingData->iTable == NULL)
				return 1;

			break;

		case D_TABLE:
			codingData->dTableSize += TABLE_BUFFER;
			codingData->dTable = (dataCell*) realloc(codingData->dTable, codingData->dTableSize);
			if (codingData->dTable == NULL)
				return 1;

			break;

		case S_TABLE:
			codingData->sTableSize += TABLE_BUFFER;
			codingData->sTable = (symbolCell*) realloc(codingData->sTable, codingData->sTableSize);
			if (codingData->sTable == NULL)
				return 1;

			break;
	}

	return 0;
}

/*void addToTable(fileCodingStruct *codingData, int tableType, ...)
{
	va_list args;


	switch (tableType)
	{
		case I_TABLE:  machineCode wasCoded 
			
			break;

		case D_TABLE:  machineCode, byteAmount 

			break;

		case S_TABLE:  *name, adress, placing, visibility 

			break;
	}
}*/

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

void pushCode(long int code, fileCodingStruct *codingData)
{
	/* Temprorary until start using tables: ############################### */
	codingData->code = code;
}

int pushLable(char *label, int placing, fileCodingStruct *codingData)
{/*	
	returns 0 on success, 1 if found error, -1 if failed to realloc symbol table size
*/
	int i=0;
	/*char *errorString[LINE_LENGTH];
	char *sourceLineString[20];*/

	/* checking if label was already defined: */
	for (i=0; i<codingData->sc; i++) 
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
	i=sc, pointing to next available spot in the table */

	if (i>=codingData->sTableSize) /* expanding symbol table if needed */
	{
		if (expandTable(S_TABLE, codingData))
			return -1; /* failed expanding tabel */
	}

	strcpy(codingData->sTable[i].name, label);
	if (placing == CODE_IMAGE)
		codingData->sTable[i].adress = codingData->ic;

	if (placing == DATA_IMAGE)
		codingData->sTable[i].adress = codingData->dc;

	codingData->sTable[i].placing = placing;
	codingData->sTable[i].visibility = INTERN;

	return 0;
}


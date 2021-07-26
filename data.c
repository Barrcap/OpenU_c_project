#include <stdlib.h>

#include "data.h"

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



void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 0;
	codingData->sourceLine = 1;
}

int getLabelAdress(char *lableName, fileCodingStruct *codingData)
{
	return 0;
}

int getIC(fileCodingStruct codingData)
{
	return 0;
}

void pushCode(long int code, fileCodingStruct *codingData)
{
	
}
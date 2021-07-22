#include <stdlib.h>

#include "data.h"

int createTables(dataCell *iTable, dataCell *dTable, symbolCell *symbolTable)
{

	iTable = (dataCell*) calloc(TABLE_BUFFER, sizeof(dataCell));
	if (iTable == NULL)
		return 1;

	dTable = (dataCell*) calloc(TABLE_BUFFER, sizeof(dataCell));
	if (dTable == NULL)
		return 1;

	symbolTable = (symbolCell*) calloc(TABLE_BUFFER, sizeof(symbolCell));
	if (symbolTable == NULL)
		return 1;

	return 0;
}


void freeTables(dataCell *iTable, dataCell *dTable, symbolCell *symbolTable)
{

	free(iTable);
	free(dTable);
	free(symbolTable);
	
}


void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 0;
	codingData->sourceLine = 1;
}
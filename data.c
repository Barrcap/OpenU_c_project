#include "data.h"

int createTables(dataCell *iTable, dataCell *dTable, symbolCell *symbolTable)
{
	return 0;
}


void freeTables(dataCell *iTable, dataCell *dTable, symbolCell *symbolTable)
{
	
}


void resetCounterParams(fileCodingStruct *codingData)
{
	codingData->ic = 100;
	codingData->dc = 0;
	codingData->sourceLine = 1;
}
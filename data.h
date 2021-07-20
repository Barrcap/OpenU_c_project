#define DATA_BUFFER 20
#define LABLE_SIZE 31


typedef struct dataCell
{
	int adress;
	char sourceCode[LINE_LENGTH];
	long int machineCode;
	char wasCoded;

}dataCell;

typedef dataCell dataArray[DATA_BUFFER];


typedef struct symbolCell
{
	char name[LABLE_SIZE];
	int value;
	unsigned char placing : 1; /* 0-code image, 1-data image */
	unsigned char visibility : 2; /* 0-internal, 1-entry, 2-extern */

}symbolCell;

typedef struct fileCodingData
{
	int ic;
	int dc;
	dataCell *iTable; /* Instructions Table */
	dataCell *dTable; /* Data Table */
	symbolCell *symbolTable;
	int sourceLine;
};
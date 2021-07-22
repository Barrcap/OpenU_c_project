#define FILE_NAME_SIZE 40
/*#define MACHINE_CODE_MAX_SIZE 1000  Supports roughly 250 lines of code 
	MACHINE_CODE_MAX_SIZE is in bytes.
	actual value is our virtual machine's memory (2^25) */
#define TABLE_BUFFER 20
#define LABLE_SIZE 31
#define LINE_LENGTH 80

#define I_TABLE 0
#define D_TABLE 1
#define S_TABLE 2



typedef struct dataCell
{
/*	Structure for Instruction Table and Data Table.
	Each cell represents a line the table.
*/
	int adress;
	char sourceCode[LINE_LENGTH];
	long int machineCode; /* using only 32 bits */
	unsigned int wasCoded : 1;
	unsigned int byteAmount : 3;
/*	byteAmount is relevant for data table, represents how many bytes are used
	out of machineCode's 4 relevant bytes: 1-one byte, 2-half word, 4-word */

}dataCell;


typedef struct symbolCell
{
/*	Structure for Symbol Table.
	Each cell represents a line in the table.
*/
	char name[LABLE_SIZE];
	int value;
	unsigned int placing : 1; /* 0-code image, 1-data image */
	unsigned int visibility : 2; /* 0-internal, 1-entry, 2-extern */

}symbolCell;

typedef struct fileCodingStruct
{
/*	
	General structure that contains all 3 data tables and relevant valiables for encoding.
	will be crated when starting to work on a file, made in order to easily give relevant
	fucntions easy access to the data.
*/
	dataCell *iTable; /* Instructions Table */
	dataCell *dTable; /* Data Table */
	symbolCell *sTable;
	int iTableSize;
	int dTableSize;
	int sTableSize;

	int ic; /* instruction commands counter */
	int dc; /* data commands counter */
	int icf; /* amount of instruction commands*/
	int dcf; /* amount of data commands */
	int sourceLine;

	char fileName[FILE_NAME_SIZE];
}fileCodingStruct;


int createTables(fileCodingStruct *codingData);
int expandTable(int tableType, fileCodingStruct *codingData);
void freeTables(fileCodingStruct *codingData);


void resetCounterParams(fileCodingStruct *codingData);
#define FILE_NAME_SIZE 40
/*#define MACHINE_CODE_MAX_SIZE 1000  Supports roughly 250 lines of code 
	MACHINE_CODE_MAX_SIZE is in bytes.
	actual value is our virtual machine's memory (2^25) */
#define TABLE_BUFFER 20
#define LINE_LENGTH 80
#define LABEL_SIZE 31
#define COMMAND_SIZE 10
#define START_LINE 100
#define DB 1
#define DH 2
#define DW 4


enum image_type {CODE_IMAGE, DATA_IMAGE};
enum symbol_visibility {INTERN, ENTRY, EXTERN};


typedef struct symbolLink
{
	/*	Structure for Symbol Table.
		Each cell represents a line in the table.
	*/
	char name[LABEL_SIZE];
	int adress;
	unsigned int placing : 1; /* 0-code image, 1-data image */
	unsigned int visibility : 2; /* 0-internal, 1-entry, 2-extern */
	struct symbolLink *next;

}symbolLink;

typedef struct fileCodingStruct
{
/*	
	General structure that contains all 3 data tables and relevant valiables for encoding.
	will be crated when starting to work on a file, made in order to easily give relevant
	fucntions easy access to the data. 
*/

	symbolLink *symbolLinkHead;

	int ic; /* instruction commands counter */
	int dc; /* data commands counter */
	int icf; /* amount of instruction commands*/
	int dcf; /* amount of data commands */
	int sourceLine;

	long int code; /* Temprorary until start using tables ############################### */

	char fileName[FILE_NAME_SIZE];
}fileCodingStruct;




void freeSymbolTable (fileCodingStruct *codingData);

void resetCounterParams(fileCodingStruct *codingData);
void advanceImageCounter(int imageType, fileCodingStruct *codingData);
int getIC(fileCodingStruct *codingData);

int analyzeCommand(char *commandName, int *imageType, int *commandImageBytes, fileCodingStruct *codingData);


int pushLable(char *label, int placing, fileCodingStruct *codingData);
int getLabelAdress(char *labelName, fileCodingStruct *codingData);
void finalizeSymbolTable(fileCodingStruct *codingData);








/* not relevant, to be deleted: ##################################################### */
int pushCode(long int code, fileCodingStruct *codingData);
/* not relevant, to be deleted end ##################################################### */



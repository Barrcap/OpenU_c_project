#define LINE_LENGTH 80
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
	unsigned char ; /* come back here!!!!!!!!*/

}symbolCell;





int fileCompiler(char *fileName);
int readFileLine(FILE *file, char *line, int *reachedEOF);
int CodingLineTake1(char *line);
int CodingLineTake2(char *line);
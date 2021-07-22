#define RESET "\033[0m" /* print color for printError */
#define RED "\033[31m" /* print color for printError */



struct fileCodingStruct;


int fileCompiler(char *fileName);
int readFileLine(FILE *file, char *line, int *reachedEOF, struct fileCodingStruct *codingData);
int CodingLineTake1(char *line);
int CodingLineTake2(char *line);

void printError(char *errorString, struct fileCodingStruct *codingData);
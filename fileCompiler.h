#define RESET "\033[0m" /* print color for printError */
#define RED "\033[31m" /* print color for printError */



struct fileCodingStruct;


int fileCompiler(char *fileName);
int readFileLine(FILE *file, char *line, int *reachedEOF, struct fileCodingStruct *codingData);
int encodingLineTake1(char *line);
int encodingLineTake2(char *line);

/*##################################################################################*/
long int roiEncoding(char *command, char *operands); /*dummy function Roi's encoding function */
/*##################################################################################*/

void printError(char *errorString, struct fileCodingStruct *codingData);
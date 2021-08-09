#define SHOW_SYMBOL_TABLE 1
#define SHOW_ENCODING 0


#define RESET "\033[0m" /* print color for printError */
#define RED "\033[31m" /* print color for printError */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */



struct fileCodingStruct;


int fileCompiler(char *fileName);
int readFileLine(FILE *file, char *line, int *reachedEOF, struct fileCodingStruct *codingData);
int encodingLineTake1(char *line, struct fileCodingStruct *codingData);
int encodingLineTake2(char *line, struct fileCodingStruct *codingData);
int seperateArguments(char *line, char *lable, char *command, char *operands, struct fileCodingStruct *codingData);
int operandPointers(char *line, int *start, int *end);
int countOperands(char *operands);
int getStringLenght(char *operands);

/*###################################################################################*/
long int roiEncoding(char *command, char *operands); /*dummy function Roi's encoding function */
/*###################################################################################*/

void printError(char *errorString, struct fileCodingStruct *codingData);
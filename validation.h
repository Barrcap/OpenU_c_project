enum print_validation_error {DONT_PRINT_ERROR, PRINT_ERROR};


struct fileCodingStruct;

int validateLabel(char *label, struct fileCodingStruct *codingData,int print_error);
int howManyComma(char * str);
int isCorrectImmed(char * immed, struct fileCodingStruct *codingData,int print_error);
int isCorrectReg(char * reg, struct fileCodingStruct *codingData,int print_error);
int validateOperands(char *operands, int validCase, struct fileCodingStruct *codingData);

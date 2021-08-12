struct fileCodingStruct;

int validateLabel(char *label, struct fileCodingStruct *codingData);
int howManyComma(char * str);
int isCorrectImmed(char * immed, struct fileCodingStruct *codingData);
int isCorrectReg(char * reg, struct fileCodingStruct *codingData);
int avner (char * reg, struct fileCodingStruct *codingData);
int validateOperands(char *operands, int validCase, struct fileCodingStruct *codingData);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ERROR -1
#define COMMAND_NUM 27

/*######### THIS IS HEADER FILE ############*/

typedef struct s1
{

	char command[30];
	char action;
	int occation;
	int funct;
	int opcode;

}line;


line lines[] = {
/*	command	action,occation,funct,opcode*/
	{"add",		'R'	,1	,1	,0},
	{"sub",		'R'	,1	,2	,0},
	{"and",		'R'	,1	,3	,0},
	{"or",		'R'	,1	,4	,0},
	{"nor",		'R'	,1	,5	,0},
	{"move",	'R'	,2	,1	,1},
	{"mvhi",	'R'	,2	,2	,1},
	{"mvlo",	'R'	,2	,3	,1},
	{"addi",	'I'	,3	,0	,10},
	{"subi",	'I'	,3	,0	,11},
	{"andi",	'I'	,3	,0	,12},
	{"ori",		'I'	,3	,0	,13},
	{"nori",	'I'	,3	,0	,14},
	{"bne",		'I'	,4	,0	,15},
	{"beq",		'I'	,4	,0	,16},
	{"blt",		'I'	,4	,0	,17},
	{"bgt",		'I'	,4	,0	,18},
	{"lb",		'I'	,3	,0	,19},
	{"sb",		'I'	,3	,0	,20},
	{"lw",		'I'	,3	,0	,21},
	{"sw",		'I'	,3	,0	,22},
	{"lh",		'I'	,3	,0	,23},
	{"sh",		'I'	,3	,0	,24},
	{"jmp",		'J'	,5	,0	,30},
	{"la",		'J'	,6	,0	,31},
	{"call",	'J'	,6	,0	,32},
	{"stop",	'J'	,7	,0	,63}
};

/*######### THIS IS HEADER FILE ############*/


/*strings that stores the string without a '$' ######there is possibilty that we have a problem this way##########*/
static char strREG1[2];
static char strREG2[2];
static char strREG3[2];

/*function that prtinting integer in binary*/
void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

char * removeDollar(char * str,int occation){
	switch(occation){
		case 1:
		strREG1[0] = str[1];
		strREG1[1] = str[2];
		return strREG1;
		case 2:
		strREG2[0] = str[1];
		strREG2[1] = str[2];
		return strREG2;
		case 3:
		strREG3[0] = str[1];
		strREG3[1] = str[2];
		return strREG3;
		default :
		printf("You entered wrong case");
		return NULL;
	}
}

int findfunct(char * str){
	/* in this case we are running from 0 to 4 index in array lines*/
	int x;
	int i;
	int res = ERROR;
	
	

	for(i=0; i<8; i++){
		if((x = strcmp(str, lines[i].command)) == 0){
			res = lines[i].funct;
			printf("####THIS IS FUNCT NUMBER\n" );
			printf("%s\n", str );
			printf("%s\n", lines[i].command );
			printf("####THIS IS FUNCT NUMBER\n" );
		}
	}
	
	return res;
}

int findOpcode(char * str){
	/* in this case we are running from 0 to 4 index in array lines*/
	int x;
	int i;
	int res = ERROR;
	
	for(i=0; i<8; i++){
		if((x = strcmp(str, lines[i].command)) == 0){
			res = lines[i].opcode;
			printf("####THIS IS opcode NUMBER\n" );
			printf("%s\n", str );
			printf("%d\n", lines[i].opcode );
			printf("####THIS IS opcode NUMBER\n" );
		}
	}
	
	return res;
}



long int firstCase_toBinary(char * str ,char * commandSTR){
	/*Extracting each register from the string*/
	char * tempReg1  = strtok(str,",");
	char * tempReg2 = strtok(NULL,",");
	char * tempReg3 = strtok(NULL,",");
	/*removing the '$ from the strings and saving in new pointer */
	char * reg1 = removeDollar(tempReg1,1);
	char * reg2 = removeDollar(tempReg2,2);
	char * reg3 = removeDollar(tempReg3,3);
	/*convert the string to an integer */
	unsigned int reg1Val = atoi(reg1);
	unsigned int reg2Val = atoi(reg2);
	unsigned int reg3Val = atoi(reg3);


	/*coding to binary*/
	long int add_code = 0;
	long int mask = findOpcode(commandSTR);        /*this is the case with opcode = 1*/ 
	mask <<=  26;
	add_code |= mask;
	mask = reg1Val;
	mask <<= 21;
	add_code |= mask;
	mask = reg2Val;
	mask <<= 16;
	add_code |= mask;
	mask = reg3Val;
	mask <<= 11;
	add_code |= mask;
	mask = findfunct(commandSTR);  /*this mask initialize funct in each case and thats the only diffrence in this case*/
	mask <<= 6;
	add_code |= mask;
	/*TESTS*/
	printf("%s\n",tempReg1 );
	printf("%s\n",tempReg2 );
	printf("%s\n",tempReg3 );
	printf("%d\n",reg1Val );
	printf("%d\n",reg2Val );
	printf("%d\n",reg3Val );
	printf("%s\n",reg1 );
	/*TESTS*/
	return add_code;


}

long int secoundCase(char * str ,char * commandSTR){
	/*Extracting each register from the string*/
	char * tempReg1  = strtok(str,",");
	char * immed = strtok(NULL,",");
	char * tempReg2 = strtok(NULL,",");
	/*removing the '$ from the strings and saving in new pointer */
	char * reg1 = removeDollar(tempReg1,1);
	char * reg2 = removeDollar(tempReg2,2);
	/*convert the string to an integer */
	int reg1Val = atoi(reg1);
	int reg2Val = atoi(reg2);
	int immedVal = atoi(immed);

	long int code = 0;
	long int mask = findOpcode(commandSTR); 
	mask <<=  26;
	code |= mask;
	mask = reg1Val;
	mask <<= 21;
	code |= mask;
	mask = reg2Val;
	mask <<= 16;
	code |= mask;

	/*we have to cheak if the number is in 2^16 range in singed method */
	/* in fact the range is 2^8 in possitive numbers and vice versa */
	mask = immed;
	code |= mask;


}





int main (){
	char string [] = "$3,$4,$5";
	char commandString [] = "move";
	int test = firstCase_toBinary(string,commandString);
	printf("%d\n", test);
	bin(test);
	return 0;
}
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

int sort(char * str ,char * commandSTR) {
	int code;
	int i = 0;
	while(strcmp(commandSTR, lines[i].command)){
		i++;
	}
	if(i <= 7){
		code = Rcase_toBinary(str,commandSTR);
	}
	else if (i >= 8 && i <= 22){
		code = Icase_toBinary(str,commandSTR);
	}
	else if (i >= 23 && i <=26 ){
		code = Jcase_toBinary(str,commandSTR);
	}
	else return -1;

	return code;

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
		}
	}
	
	return res;
}

int findOpcode(char * str){
	/* in this case we are running from 0 to 4 index in array lines*/
	int x;
	int i;
	int res = ERROR;
	
	for(i=0; i<28; i++){
		if((x = strcmp(str, lines[i].command)) == 0){
			res = lines[i].opcode;
		}
	}
	
	return res;
}



long int Rcase_toBinary(char * str ,char * commandSTR){
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
	long int code = 0;
	long int mask = findOpcode(commandSTR);        /*this is the case with opcode = 1*/ 
	mask <<=  26;
	code |= mask;
	mask = reg1Val;
	mask <<= 21;
	code |= mask;
	mask = reg2Val;
	mask <<= 16;
	code |= mask;
	mask = reg3Val;
	mask <<= 11;
	code |= mask;
	mask = findfunct(commandSTR);  /*this mask initialize funct in each case and thats the only diffrence in this case*/
	mask <<= 6;
	code |= mask;
	return code;


}

long int Icase(char * str ,char * commandSTR){            /* this method isnt ready yet*/ /*have to chek complete to 2 method*/
	/*Extracting each register from the string*/
	int tempHex = 0x00FFFFFF;
	char * tempReg1  = strtok(str,",");
	char * immed = strtok(NULL,",");
	char * tempReg2 = strtok(NULL,",");
	/*removing the '$ from the strings and saving in new pointer */
	char * reg1 = removeDollar(tempReg1,1);
	char * reg2 = removeDollar(tempReg2,2);
	/*convert the string to an integer */
	int reg1Val = atoi(reg1);
	int reg2Val = atoi(reg2);
	short immedVal = atoi(immed);

	long int code = 0;
	int opcode = findOpcode(commandSTR);
	long int mask = opcode; 
	mask <<=  26;
	code |= mask;
	mask = reg1Val;
	mask <<= 21;
	code |= mask;
	mask = reg2Val;
	mask <<= 16;
	code |= mask;
	if(opcode >= 10 && opcode <= 14 || opcode >= 19 && opcode <= 24){
		mask = tempHex;
		mask = mask & immedVal;
		code |= mask;
	}
	else if(opcode >= 15 && opcode <= 18){
		/*no clear how to write this code for this time*/
		/*we need lable table*/
	}

	return code;

}

long int Jcase(char * str ,char * commandSTR){    /*this function isnt ready yet*/
	/*Extracting each register from the string*/
	if(!isLabel()){ 									/* i have to write  function who can find if this is label*/
		int tempHex = 0x00FFFFFF;
		char * reg1 = removeDollar(str,1);
		/*convert the string to an integer */
		short reg1Val = atoi(reg1);
		long int code = 0;
		int opcode = findOpcode(commandSTR);
		long int mask = opcode; 
		mask <<=  26;
		code |= mask;
		mask = 1;        /*this is register case*/
		mask <<= 21;
		code |= mask;
		mask = tempHex;
		mask = mask & reg1Val;
		code |= mask;
		return code;
	}
	else {

		/* this is lable case*/
	}
	
}



int main (){
	char string [] = "$3,$4,$5";
	char commandString [] = "move";
	int test = firstCase_toBinary(string,commandString);
	printf("%d\n", test);
	bin(test);
	return 0;
}
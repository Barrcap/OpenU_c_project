#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "toBinary.h"
#include "command_table.h"
#include "data.h"
#include "fileCompiler.h"







/*function that prtinting integer in binary*/
void binPrint(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
    printf("\n");
}

int isLabel(char * str){

	if (str[0] == '$')
		return 0;
	else
		return 1;
}

int toBinary(char * str ,char * commandSTR, fileCodingStruct *codingData) {
	long int code;
	int i = 0;
	while(strcmp(commandSTR, lines[i].command)){
		i++;
	}
	if(i <= 7){
		code = Rcase(str,commandSTR, codingData);
	}
	else if (i >= 8 && i <= 22){
		code = Icase(str,commandSTR, codingData);
	}
	else if (i >= 23 && i <=26 ){
		code = Jcase(str,commandSTR, codingData);
	}
	else
	{
		printError("invalid command",codingData);
		return 1;
	}

	pushCode(code, codingData);
	return 0;

}

/*void removeDollar(char * str,int occation, char *strREG1, char *strREG2, char strREG3)*/
void removeDollar(char *str, char *strREG){

	strREG[0] = str[1];
	strREG[1] = str[2];
	/*
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
	}*/
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



long int Rcase(char * str ,char * commandSTR, fileCodingStruct *codingData){
	/*Extracting each register from the string*/
	char reg1[REG_LENGHT] = {0};
	char reg2[REG_LENGHT] = {0};
	char reg3[REG_LENGHT] = {0};

	unsigned int reg1Val;
	unsigned int reg2Val;
	unsigned int reg3Val;

	long int code;
	long int mask; 

	char * tempReg1  = strtok(str,",");
	char * tempReg2 = strtok(NULL,",");
	char * tempReg3 = strtok(NULL,",");
	/*removing the '$ from the strings and saving in new pointer */

	/*reg1 = removeDollar(tempReg1,1);
	reg2 = removeDollar(tempReg2,2);
	reg3 = removeDollar(tempReg3,3);*/


	removeDollar(tempReg1, reg1);
	removeDollar(tempReg2, reg2);
	removeDollar(tempReg3, reg3);
	/*convert the string to an integer */
	reg1Val = atoi(reg1);
	reg2Val = atoi(reg2);
	reg3Val = atoi(reg3);


	/*coding to binary*/
	code = 0;
	mask = findOpcode(commandSTR);        /*this is the case with opcode = 1*/ 
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

long int Icase(char * str ,char * commandSTR, fileCodingStruct *codingData){            /* this method isnt ready yet*/ /*have to chek complete to 2 method*/
	/*Extracting each register from the string*/
	int tempHex = 0x00FFFFFF;
	char * tempReg1  = strtok(str,",");
	char * immed = strtok(NULL,",");
	char * tempReg2 = strtok(NULL,",");
	/*removing the '$ from the strings and saving in new pointer */

	char reg1[REG_LENGHT] = {0};
	char reg2[REG_LENGHT] = {0};

	int reg1Val;
	int reg2Val;

	short immedVal;
	long int code;
	int opcode;
	long int mask;

	removeDollar(tempReg1, reg1);
	removeDollar(tempReg2, reg2);

	/*convert the string to an integer */
	reg1Val = atoi(reg1);
	reg2Val = atoi(reg2);
	immedVal = atoi(immed);

	code = 0;
	opcode = findOpcode(commandSTR);
	mask = opcode; 



	mask <<=  26;
	code |= mask;
	mask = reg1Val;
	mask <<= 21;
	code |= mask;
	mask = reg2Val;
	mask <<= 16;
	code |= mask;
	if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)){
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

long int Jcase(char * str ,char * commandSTR, fileCodingStruct *codingData){    /*this function isnt ready yet*/
	/*Extracting each register from the string*/
	if(!isLabel(str)){ 									
		int tempHex = 0x00FFFFFF;

		char reg1[REG_LENGHT];
		short reg1Val;
		long int code;
		int opcode;
		long int mask;

		removeDollar(str, reg1);

		/*convert the string to an integer */
		reg1Val = atoi(reg1);
		code = 0;
		opcode = findOpcode(commandSTR);
		mask = opcode; 
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

		return 0;
		/* this is lable case*/
	}
	
}


/*
int main (){
	char string [] = "$3,$4,$5";
	char commandString [] = "move";
	int test = Rcase(string,commandString);
	printf("%d\n", test);
	bin(test);
	return 0;
}*/
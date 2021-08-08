#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "toBinary.h"
#include "commandTable.h"
#include "data.h"
#include "fileCompiler.h"
#include <ctype.h>


/*function that prtinting integer in binary*/
void binPrint(unsigned long int n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
    printf("\n");
}

int isLabel(char * str){

	if (str[0] == '$')
		return 0;
	else if(str[0] == '-')              /*thats the only cases to be a register, all the rest cases is  lables */
		return 0;
	else if(isdigit(str[0]))
		return 0;
	else
		return 1;
}

int getDistAddres(char * label, fileCodingStruct *codingData){
	int destAdd = getLabelAdress(label, codingData);
	int sourceAdd = getIC(codingData); /*find my address*/
	int res = destAdd - sourceAdd; /*this is the clac to know where we have to go*/
	return res;
}



int toBinary(char * commandSTR, char * operands, fileCodingStruct *codingData) {
	int res;
	int i = 0;
	while(strcmp(commandSTR, lines[i].command)){
		i++;
	}
	if(i <= 4){
		res = Rcase(operands,commandSTR, codingData);
	}
	else if (i >= 5 && i <= 7){
		res = RRcase(operands,commandSTR, codingData);
	}
	else if (i >= 8 && i <= 22){
		res = Icase(operands,commandSTR, codingData);
	}
	else if (i >= 23 && i <=26 ){
		res = Jcase(operands,commandSTR, codingData);
	}
	else
	{
		printError("invalid command",codingData);
		return 1;
	}

	return res;
}

/*void removeDollar(char * str,int occation, char *strREG1, char *strREG2, char strREG3)*/
void removeDollar(char *str, char *strREG){

	strREG[0] = str[1];
	strREG[1] = str[2];
	
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

	pushCode(code, codingData); 
	return 0;

}

long int RRcase(char * str ,char * commandSTR, fileCodingStruct *codingData){
	/*Extracting each register from the string*/
	char reg1[REG_LENGHT] = {0};
	char reg2[REG_LENGHT] = {0};

	unsigned int reg1Val;
	unsigned int reg2Val;

	long int code;
	long int mask; 

	char * tempReg1  = strtok(str,",");
	char * tempReg2 = strtok(NULL,",");

	removeDollar(tempReg1, reg1);
	removeDollar(tempReg2, reg2);

	reg1Val = atoi(reg1);
	reg2Val = atoi(reg2);

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
	mask = 0;
	mask <<= 11;
	code |= mask;
	mask = findfunct(commandSTR);  /*this mask initialize funct in each case and thats the only diffrence in this case*/
	mask <<= 6;
	code |= mask;

	pushCode(code, codingData); 
	return 0;

}

long int Icase(char * str ,char * commandSTR, fileCodingStruct *codingData){            /* this method isnt ready yet*/ /*have to chek complete to 2 method*/
	long int tempHex = 0x0000FFFF;    /*this varieble helps to initilaize immed field */
	char * tempReg1  = strtok(str,",");
	char * immed = strtok(NULL,",");
	char * tempReg2 = strtok(NULL,",");
	char reg1[REG_LENGHT] = {0};
	char reg2[REG_LENGHT] = {0};
	int reg1Val;
	int reg2Val;
	short immedVal;
	long int code;
	int opcode;
	long int mask;
	long int tempMask;  /*this varieabl containes the val on each case*/
	short distance;  /*the distance form labels*/
	removeDollar(tempReg1, reg1);
	removeDollar(tempReg2, reg2);


	/*convert the string to an integer */
	reg1Val = atoi(reg1);
	reg2Val = atoi(reg2);
		/*this way we are checking what case we are (register or lables) */
	if(!isLabel(immed)){
		immedVal = atoi(immed);
		mask = tempHex;
		tempMask = mask & immedVal;
	}
	else{
		distance = getDistAddres(immed, codingData);
		mask = tempHex;
		tempMask = mask & distance;
	}

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
	code |= tempMask; /*this is the coding for each case*/
	

	pushCode(code, codingData); 
	return 0;

}

long int Jcase(char * str ,char * commandSTR, fileCodingStruct *codingData){    /*this function isnt ready yet*/
	/*Extracting each register from the string*/
	long int tempHex = 0x00FFFFFF; /*this varieble helps to initilaize immed field */
	char address[REG_LENGHT];
	short reg1Val;
	long int code;
	int opcode;
	long int mask;
	int addressVal;

	

	if(!isLabel(str)){ 									
		removeDollar(str, address);
		/*convert the string to an integer */
		reg1Val = atoi(address);
		code = 0;
		opcode = findOpcode(commandSTR);
		mask = opcode; 
		mask <<=  26;
		code |= mask;
		mask = 1;        /*this is register case*/
		mask <<= 25;
		code |= mask;
		mask = tempHex;
		mask = mask & reg1Val;
		code |= mask;

		pushCode(code, codingData); 
		return 0;
	
	}
	else {
		addressVal = getLabelAdress(str , codingData);
		code = 0;
		opcode = findOpcode(commandSTR);
		mask = opcode; 
		mask <<=  26;
		code |= mask;
		mask = tempHex;
		mask = mask & addressVal;
		code |= mask;

		pushCode(code, codingData); 
		return 0;

		
	}

	return 0;
	
}

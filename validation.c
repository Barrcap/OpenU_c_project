#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileCompiler.h"
#include "command_table.h"
#include "data.h"
#define LABEL_LEN 30

/*void removeColon(char *label_without_colon , char * label){
	int i =0;
	while (i <= strlen(label) -1){
		label_without_colon[i] = label[i];
		i++;
	}
} */


 int validateLabel(char *label, struct fileCodingStruct *codingData){
 	int i =0;   /*this is index varieble for while loop*/ 
 	int is_command =0;
 	char label_without_colon[29];  /*this is the maxinmun possible length */
 	/*removeColon(label_without_colon,label);  /*usefull to cheak if the label is command*/

 		/*if the first letter isnt alph print error*/ 
 	if(!isalpha(label[0])){     
 		printError("First letter isn't alph letter", codingData);
 		return 1;
 	}
 			/*in thisx case the string is to long*/
 	if(strlen(label) > LABEL_LEN){      
 		printError("string is too long", codingData);
 		return 1;
 	}

 		/*cheaking if the name of the label is command*/
 	while(i <= strlen(lines)){
 		if(strcmp(label_without_colon, lines[i].command)){
 			is_command = 1;
 		}
		i++;
	}
	if(is_command){
		printError("label name can not be command name", codingData);
		return 1;
	}
	   /*cheaking if the name of the label is command*/

	return 0;

 }
 		/*this function check how mant commas we got in the string*/
 int howManyComma(char * str){   
 	int i = 0;
 	int count =0;
 	while (i <= strlen(str)){
 		if(str[i] == ','){
 			count ++;
 		}
 		i++
 	}
 	return count;
 }

 int immedValid(char * immed){
 	int res = atoi(immed);
 	return res;
 }

 int isInRange(char * reg){
 	int numCount = 0;
 	int regVal;
 	int i = 0;
 	char reg_without_dollar[];

 	if(strlen(reg) <=1){
 		printError("missing value to the register", codingData);
 		return 1
 	}
 		/*cleaning first white letters*/
 	while(isspace(reg)){
 		i++
 	}

 	/*we are cheaking if the is '$' in each of the registers*/
	if(reg[i] != '$'){
		printError("missing $ in one of the registers", codingData);
		return 1;
	}

 	/*remove the dollar and cheaking if the value after the dollar is digit*/
 	while(i <= strlen(reg) -1){
 		if(numCount > 2){
 			printError("invalid letter", codingData);
 			return 1
 		}
 		if(isdigit(reg[i+1])){
 			reg_without_dollar[i] = reg[i +1];
 			numCount ++;
 			i++;
 		}
 		else{
 			printError("invalid letter ", codingData);
 			return 1;
 		}	
 	}


 	/*check the value of the register*/ 
 	regVal = atoi(reg_without_dollar);
 		/*we are cheaking if the value is on range*/
 	if((!regVal >=0) || (!regVal <=31)){
 		printError("the value of the register is out of range", codingData);
 		return 1
 	}

 	else
 		return 0;

 }

  int validateOperands(char *operands, int validCase, struct fileCodingStruct *codingData){
  			/*creating template for the registers*/
  	char reg1[strlen(operands)] = {0};
	char reg2[strlen(operands)] = {0};
	char reg3[strlen(operands)] = {0};
	char immed[strlen(operands)] = {0};

  	char tempOperands[strlen(operands)];
  	tempOperands = strcpy(tempOperands, operands);      /*we need copy of the main string */

  	switch(validCase){

  		case 1: 
  			if(howManyComma(tempOperands) != 2){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}

  			 /*we are split the string to 3 parts in this case*/
  			reg1  = strtok(tempOperands,",");
			reg2 = strtok(NULL,",");
			reg3 = strtok(NULL,",");

			
			if(isInRange(reg1) && isInRange(reg2) && isInRange(reg3)){
				return 0;
			}

			else
				return 1;


		case 2:
			if(howManyComma(tempOperands) != 1){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}	

  			/*we are split the string to 2 parts in this case*/
  			reg1  = strtok(tempOperands,",");
			reg2 = strtok(NULL,",");

			if(isInRange(reg1) && isInRange(reg2)){
				return 0;
			}

			else
				return 1;

		case 3:	
			if(howManyComma(tempOperands) != 2){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}

  			 /*we are split the string to 3 parts in this case*/
  			reg1  = strtok(tempOperands,",");
			immed = strtok(NULL,",");
			reg3 = strtok(NULL,",");

			if(isInRange(reg1) && immedValid(immed) && isInRange(reg3)){
				return 0;
			}
			else
				return 1;

			

			case 4:







  	}

  }


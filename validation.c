#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fileCompiler.h"
#include "commandTable.h"
#include "data.h"



 int validateLabel(char *label, struct fileCodingStruct *codingData)
 {
 	int i =0;   /*this is index varieble for while loop*/ 
 	int is_command =0;
 	char label_without_colon[29];  /*this is the maxinmun possible length */
	/*usefull to cheak if the label is command*/

 		/*if the first letter isnt alph print error*/ 
 	if(!isalpha(label[0])){     
 		printError("First letter isn't alph letter", codingData);
 		return 1;
 	}
 			/*in thisx case the string is to long*/
 	if(strlen(label) >= LABEL_SIZE){      
 		printError("string is too long", codingData);
 		return 1;
 	}

 		/*cheaking if the name of the label is command*/
 	while(i <= COMMAND_NUM){
 		if(strcmp(label, lines[i].command) == 0){
 			is_command = 1;
 			break;
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

 int howManyComma(char * str)
 {   
 	int i = 0;
 	int count =0;
 	while (i <= strlen(str)){
 		if(str[i] == ','){
 			count ++;
 		}
 		i++;
 	}
 	return count;
 }

 int isCorrectImmed(char * immed)   
 {
 	char tempImmed [80];    /*80 is the max possible length of a string*/
 	char strPtr[80];   /*pointer for srtol*/
 	int num;
 	int strPtr_index = 0;   
 	int index = 0;
 	int boolean = 0;

 	strcpy(tempImmed,immed);   /*have to this because we dont want to destroied the original string*/

 			/*we are check the first part of the string for abc0 case*/
 	while(index < strlen(tempImmed)){
 		if(isspace(tempImmed[index])){
 			i++;
 		}
 		else if((tempImmed[index] == '-') || (tempImmed[index] == '+')){

 			if(boolean == 1){
 				printError("immed value is incorrect", codingData);  /*in this case this is the secound time that we are seeing + or -*/
 				return 0;
 			}
 				
 			boolean = 1;
 			i++;
 		}
 		else if(isdigit(tempImmed[index])){
 			break;
 		}
 		else{
 			printError("immed value is incorrect", codingData);
 			return 0;
 		}	
 	}

 	num = strtol(tempImmed, &strPtr, 10);   /*now the first part of the string is a number and the secound is unknowed*/
 	
 	if(strPtr != NULL){     /*its mark that the secound field is with alphbetic letters or with white letters*/
 			if(isspace(strPtr[strPtr_index])){   /*if the first letter is white letter we have to chek if all the part is white spaces*/
 				while(strPtr_index < strlen(strPtr)){
 					if(isspace(strPtr[strPtr_index])){
 						i++;
 					}
 					else{      /*if its nnot white letter, the only possible case is invalid letter*/
 						printError("immed value is incorrect", codingData);
 						return 0;		
 					}
 				}
 			}
 			else {
 				printError("immed value is incorrect", codingData);   
 				return 0;
 			}

 	}

 	return 1;


 }

 int isCorrectReg(char * reg, struct fileCodingStruct *codingData)
 {
 	int numCount = 0;
 	int regVal = 0;
 	int i = 0;
 	int spaces_boolean = 0;
 	char reg_without_dollar[10]; /* check buffer number ############################### */

 	if(strlen(reg) <=1){
 		printError("missing value to the register", codingData);
 		return 1;
 	}
 		/*cleaning first white letters*/
 	while(isspace(reg[i])){
 		i++;
 	}

 	/*we are cheaking if the is '$' in each of the registers*/
	if(reg[i] != '$'){
		printError("missing $ in one of the registers", codingData);
		return 1;
	}

 	/*remove the dollar and cheaking if the value after the dollar is digit*/
 	while(i <= strlen(reg) -1){
 		if(numCount > 2){     /*num count is he maximun digit we can contain in the register*/
 			printError("invalid letter", codingData);
 			return 1;
 		}
 		if(isdigit(reg[i+1]) && (spaces_boolean == 0)){
 			reg_without_dollar[i] = reg[i +1];
 			numCount ++;
 			i++;
 		}
 		/*we can get spaces after the number - for example:"$30  ' ". so in this case we are using bolean that we are saw space, so now we cant see a digit */
 		else if(isspace(reg[i+1])){    
 			spaces_boolean =1;
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
 	if((regVal < 0) || (regVal > 31)){
 		printError("the value of the register is out of range", codingData);
 		return 1;
 	}

 	else
 		return 0;

 }


 

  int validateOperands(char *operands, int validCase, struct fileCodingStruct *codingData)
  {
  			/*creating template for the registers*/
  	char *reg1;
	char *reg2;
	char *reg3;
	char *immed;
	char *string;

  	char tempOperands[LINE_LENGTH];
  	strcpy(tempOperands, operands);      /*we need copy of the main string */

  	switch(validCase){

  		case 1: 
  			if(howManyComma(tempOperands) != 2){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}

  			 /*we are split the string to 3 parts in this case*/
  			reg1 = strtok(tempOperands,",");
			reg2 = strtok(NULL,",");
			reg3 = strtok(NULL,",");

			
			if(isCorrectReg(reg1, codingData) && isCorrectReg(reg2, codingData) && isCorrectReg(reg3, codingData)){
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

			if(isCorrectReg(reg1, codingData) && isCorrectReg(reg2, codingData)){
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

			if(isCorrectReg(reg1, codingData) && isCorrectImmed(immed) && isCorrectReg(reg3, codingData)){
				return 0;
			}
			else
				return 1;

			

			case 4:
				if(howManyComma(tempOperands) != 2){     /*in this case we need 3 parts of it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;

  				/*we are split the string to 3 parts in this case*/
  				reg1 = strtok(tempOperands,",");
				reg2 = strtok(NULL,",");
				string = strtok(NULL,",");

				if(isCorrectReg(reg1, codingData) && validateLabel(string) && isCorrectReg(reg3, codingData)){
					return 0;
				}
				else
					return 1;
				
			case 5:
				if(!howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}

  				if(!isCorrectReg(tempOperands) || !validateLabel(tempOperands)){  /*we are cheaking if the string is not valid*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  				return 0;

  			case 6:	   
  				if(!howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}

  				if(!validateLabel(tempOperands)){  /*we are cheaking if the string is not valid*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  				return 0;

  			case 7:
  				if(!howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  				if(tempOperands == NULL){
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  				return 0;

  				


					

  	}


  	return 0;

  }


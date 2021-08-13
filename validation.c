#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "validation.h"
#include "fileCompiler.h"
#include "commandTable.h"
#include "data.h"
#include "debugFlags.h"


int validateLabel(char *label, struct fileCodingStruct *codingData)
{
	int i =0;  /*this is index for label*/
	int j = 0; /*this is index for command array*/
	int k = 0; /*this is index for "label_without_spaces*/    
	int is_command =0;

	char label_without_spaces[80] = {0};

	/*clean white lettters*/
	while(isspace(label[i])){
 		i++;
 			
 	}

		/*if the first letter isnt alph print error*/ 
	if(!isalpha(label[i]))
	{     
		printError("First letter isn't alph letter", codingData);
		return 1;
	}
	
			/*in thisx case the string is to long*/
	if(strlen(label) >= LABEL_SIZE)
	{      
		printError("string is too long", codingData);
		return 1;
	}

	/*we have to copy the string without the spaces because we want to compare strings*/
	while(i < strlen(label)){

		/*now we cant seeing white letters anymore cuz we getting this string with now white letters initioaly*/
		if(isspace(label[i])){
			printError("Can not use white letter at the middle of the label !", codingData);
			return 1;
		}
		if ( !(isalpha(label[i])||isdigit(label[i])) )
		{
			printError("Illegal label - invalid letter", codingData);
			return 1;
		}

		label_without_spaces[k] = label[i];
		i++;
		k++;
	}

		/*cheaking if the name of the label is command*/
	while(j <= COMMAND_NUM)
	{
		if(strcmp(label_without_spaces, lines[j].command) == 0)
		{
			is_command = 1;
			break;
		}
		j++;
	}
	j=0;
	while(j <= DATA_COMMANDS)
	{
		if(strcmp(label_without_spaces, dataCommands[j].name +1) == 0)
		{
			is_command = 1;
			break;
		}
		j++;
	}
	if(is_command)
	{
		printError("label name can not be command name", codingData);
		return 1;
	}
	   /*cheaking if the name of the label is command*/

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
	char *param;

	int operands_number; 
	int i;
	int smallestVal;
  	int biggestVal;
  	int num;

  	char tempOperands[LINE_LENGTH];
  	strcpy(tempOperands, operands);      /*we need copy of the main string */

  	if (validCase != 7 && tempOperands[0] == 0)
  	{
  		printError("Missing operands after command", codingData);
  		return 1;
  	}

  	if (SHOW_VALID_CASE) printf("Case #%i\n", validCase); /* debug printing */
  	switch(validCase){

  		case 1: /* 3 registers */
  			if(howManyComma(tempOperands) != 2){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}

  			 /*we are split the string to 3 parts in this case and making sure no operands are missing*/
  			reg1 = strtok(tempOperands,",");
  			if (reg1 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			reg2 = strtok(NULL,",");
			if (reg2 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			reg3 = strtok(NULL,",");
			if (reg3 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			

			if(!isCorrectReg(reg1, codingData) && !isCorrectReg(reg2, codingData) && !isCorrectReg(reg3, codingData))
				return 0;

			else
				return 1;



		case 2: /* 2 registers */
			if(howManyComma(tempOperands) != 1){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}	

  			/*we are split the string to 2 parts in this case and making sure no operands are missing*/
  			reg1  = strtok(tempOperands,",");
  			if (reg1 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			reg2 = strtok(NULL,",");
			if (reg2 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}

			if(!isCorrectReg(reg1, codingData) && !isCorrectReg(reg2, codingData)){
				return 0; 
			}	
			else
				return 1;

		case 3:	/* reg, immed, reg */
			if(howManyComma(tempOperands) != 2){     /*in this case we need 3 registers' it means 2 commas in the string*/
  				printError("invalid number of commas", codingData);
  				return 1;
  			}

  			 /*we are split the string to 3 parts in this case and making sure no operands are missing*/
  			reg1  = strtok(tempOperands,",");
  			if (reg1 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			immed = strtok(NULL,",");
			if (immed == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}
			reg3 = strtok(NULL,",");
			if (reg3 == NULL)
  			{
  				printError("Missing operands", codingData);
  				return 1;
  			}

			if(!isCorrectReg(reg1, codingData) && !isCorrectImmed(immed, codingData) && !isCorrectReg(reg3, codingData)){
				return 0;
			}
			else
				return 1;

			

			case 4: /* reg, reg, label */
				if(howManyComma(tempOperands) != 2){     /*in this case we need 3 parts of it means 2 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}	
  				/*we are split the string to 3 parts in this case and making sure no operands are missing*/
  				reg1 = strtok(tempOperands,",");
  				if (reg1 == NULL)
	  			{
	  				printError("Missing operands", codingData);
	  				return 1;
	  			}
				reg2 = strtok(NULL,",");
				if (reg2 == NULL)
	  			{
	  				printError("Missing operands", codingData);
	  				return 1;
	  			}
				string = strtok(NULL,",");
				if (string == NULL)
	  			{
	  				printError("Missing operands", codingData);
	  				return 1;
	  			}

				if(!isCorrectReg(reg1, codingData) && !isCorrectReg(reg2, codingData) && !validateLabel(string, codingData)){
					return 0;
				}
				else
					return 1;
				
			case 5: /* label or register */
				if(howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  						/*we are checking if this is label or register*/
  				if(isCorrectReg(tempOperands, codingData)){   /* if this is not register*/
  					if(validateLabel(tempOperands, codingData)){   /*check if this is label and if its not print error*/
  						printError("invalid label or operands", codingData);
  						return 1;
  					}			
  				}

  				if(validateLabel(tempOperands, codingData)){   /* if this is not label*/
  					if(isCorrectReg(tempOperands, codingData)){   /*check if this is register and if its not print error*/
  						printError("invalid label or operands", codingData);
  						return 1;
  					}			
  				}

  				return 0;

  			case 6: /* label */
  				if(howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}

  				if(validateLabel(tempOperands, codingData)){  /*we are cheaking if the string is not valid*/
  					printError("invalid label", codingData);
  					return 1;
  				}
  				return 0;

  			case 7: /* stop command, no operands */
  				if(howManyComma(tempOperands)){     /*in this case we need one part it means 0 commas in the string*/
  					printError("invalid number of commas", codingData);
  					return 1;
  				}
  				if(tempOperands[0] == 0){   /*we are checking if the string is null*/
  					printError("invalid line", codingData);
  					return 1;
  				}
  				return 0;

  			case 8:		/* .dh */
  			case 9:		/* .dw */
  			case 10:	/* .db */


  				switch (validCase)
  				{
  					case 8:  /*this is "dh" case*/
  						smallestVal = -32767;
  						biggestVal = 32766;
  						break;
  					case 9:   /*this is "dw" case*/
  						smallestVal = -2147483647;
  						biggestVal = 2147483646;
  						break;
  					case 10:  /*this is "db" case*/
  						smallestVal = -127;
  						biggestVal = 126;
  						break;
  				}

  				if(tempOperands[0] == ','){
  					printError("invalid string, comma at the beggining !", codingData);
  					return 1;
  				}
  				if(tempOperands[strlen(tempOperands) -1] == ','){
  					printError("invalid string, comma at the end !", codingData);
  					return 1;
  				}
  					
  				operands_number = howManyComma(tempOperands) + 1;    /*checking how many operands*/

  				/* making sure no operands are missing */
  				param = strtok(tempOperands,",");
  				if (param == NULL)
	  			{
	  				printError("Missing operands", codingData);
	  				return 1;
	  			}

  				if(isCorrectImmed(param,codingData)){     /*if its invalid param print error*/
  					printError("invalid number", codingData);
  					return 1;
  				}

  				for (i=1; i<operands_number; i++)
  				{
  					param = strtok(NULL,",");
  					if (param == NULL)
		  			{
		  				printError("Invalid comma", codingData);
		  				return 1;
		  			}

  					if(isCorrectImmed(param,codingData)){     /*if its invalid param print error*/
  						printError("invalid number", codingData);
  						return 1;
  					}

  					num = atoi(param);
  					if(num <= smallestVal || num >= biggestVal){
  						printError("number is out of range", codingData);
  						return 1;
  					}

  				}

  				return 0;


  			case 11: /* .asciz command */
  				if(strlen(tempOperands)<2 || (tempOperands[0] != '"') || (tempOperands[strlen(tempOperands) -1] != '"' )){
  					printError("invalid string !", codingData);
  					return 1;
  				}

  				return 0;

  			case 12: /* .extern and .entry commands  */

  				return validateLabel(tempOperands, codingData);

  			default:
  				printf("Bug!!!!! invalid validation case passed to validateOperands function\n");
  				return 1;
			

  	}


  	
  	return 0;
  }
  


int isCorrectImmed(char * immed, struct fileCodingStruct *codingData)   
{
 	char tempImmed [80];    /*80 is the max possible length of a string*/
 	char * strPtr;   /*pointer for srtol*/
 	int strPtr_index = 0;   
 	int index = 0;
 	int boolean = 0;
 	strcpy(tempImmed,immed);   /*have to this because we dont want to destroied the original string*/

 			/*we are check the first part of the string for abc0 case*/
 	while(index < strlen(tempImmed)){
 		if(isspace(tempImmed[index])){
 			index++;
 		}
 		else if((tempImmed[index] == '-') || (tempImmed[index] == '+')){

 			if(boolean == 1){
 				printError("invalid immed value", codingData);  /*in this case this is the secound time that we are seeing + or -*/
 				return 1;
 			}
 				
 			boolean = 1;
 			index++;
 		}
 		else if(isdigit(tempImmed[index])){
 			break;
 		}
 		else{
 			printError("invalid immed value", codingData);
 			return 1;
 		}	
 	}

 	strtol(tempImmed, &strPtr, 10);   /*now the first part of the string is a number and the secound is unknowed*/

 	if(strPtr[0] != 0){     /*its mark that the secound field is with alphbetic letters or with white letters*/
 			if(isspace(strPtr[strPtr_index])){   /*if the first letter is white letter we have to chek if all the part is white spaces*/
 				while(strPtr_index < strlen(strPtr)){
 					if(isspace(strPtr[strPtr_index])){
 						strPtr_index++;
 					}

 					else{      /*if its nnot white letter, the only possible case is invalid letter*/
 						printError("invalid immed value", codingData);
 						return 1;		
 					}
 				}
 			}

 			else{
 				printError("invalid immed value", codingData);
 				return 1;		
 			}
 			
 	}

 	return 0;
}

int isCorrectReg(char * reg, struct fileCodingStruct *codingData)
{
 	int numCount = 0;
 	int regVal = 0;
 	int j = 0;
 	int i = 0;
 	int spaces_boolean = 0;
 	char reg_without_dollar[10]={0}; /* check buffer number ############################### */

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
		printError("register not starting with $", codingData);
		return 1;
	}

 	/*remove the dollar and cheaking if the value after the dollar is digit*/
 	while(i < strlen(reg) -1){
 		if(numCount > 2){     /*num count is he maximun digit we can contain in the register*/
 			printError("invalid letter", codingData);
 			return 1;
 		}
 		if(isdigit(reg[i+1]) && (spaces_boolean == 0)){
 			reg_without_dollar[j] = reg[i +1];
 			numCount ++;
 			i++;
 			j++;
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

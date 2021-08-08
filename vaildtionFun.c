#include <stdio.h>
#include <string.h>
#define LABELS_NUM 8
#define COMMAND_NUM 28

/*define command and labels array for comparison */
char commandArray[COMMAND_NUM] = {"dummy","add","sub","and","or","nor","move","mvhi","mvlo","addi","subi","andi","ori","nori","bne","beq"
,"blt","bgt","lb","sb","lw","sw","lh","sh","jmp","la","call","stop"};


int isCommand(char * string){
	int i;
	for(i=1; i<COMMAND_NUM; i++){
		if(strcmp(string, commandArray[i]) == 0){
			return i;
		}
		else{
			return 0;
		}
	}

}


int isLineValid (char * string){
	char * str;
	str = string;
	char command[20];
	int i;

	for(i=0; !isspace(str[i]); i++){
		command[i] = str[i];
	}

	isLable(command);

	switch(int command_number = isCommand(command)){ /* check wich command it is and if this is valid command */
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		char register1[3];
		char register2[3];
		char register3[3];
		reg1_index=0;
		while(!isspace(str[i])){
			if(str[i] != '$'){
				return 1;
			}
			if(reg1_index == 1 || reg1_index == 2 && isdigit(str[i])){
				register1[reg1_index] = str[i];
			}
			if(str)
		
			reg1_index ++;
			i++;
		}
	}

	for(i =0; i<str.length(); i++){
		if(str[i] == ';'){
			return 0;
		}

		if(!isspace(str[i])){
			int word_index = 0;
			command[word_index] = str[i];
		}
	}

}



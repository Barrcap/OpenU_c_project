#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*strings that stores the string without a '$' */
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



long int add_toBinary(char * str){
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
	long int mask = reg1Val;
	mask <<= 21;
	add_code |= mask;
	mask = reg2Val;
	mask <<= 16;
	add_code |= mask;
	mask = reg3Val;
	mask <<= 11;
	add_code |= mask;
	mask = 1;
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
int main (){
	char string [] = "$3,$4,$30";
	int test = add_toBinary(string);
	printf("%d\n", test);
	bin(test);
	return 0;
}
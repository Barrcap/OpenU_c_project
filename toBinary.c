#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char str1[2];

void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

char * removeDollar(char * str){
	
	str1[0] = str[1];
	str1[1] = str[2];
	return str1;
}


long int add_toBinary(char * str){
	/*Extracting each register from the string*/
	char * tempReg1  = strtok(str,",");
	char * tempReg2 = strtok(NULL,",");
	char * tempReg3 = strtok(NULL,",");
	char * reg1 = removeDollar(tempReg1);
	char * reg2 = removeDollar(tempReg2);
	char * reg3 = removeDollar(tempReg3);
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
	
	printf("%s\n",tempReg1 );
	printf("%d\n",reg1Val );
	printf("%s\n",reg1 );
	return add_code;


}
int main (){
	char string [] = "$3,$4,$5";
	int test = add_toBinary(string);
	printf("%d\n", test);
	bin(test);
	return 0;
}
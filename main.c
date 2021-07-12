#include <stdio.h>
#include <string.h>
#include "main.h"

int main(int argc, char const *argv[])
{
	
	int i, j, isDuplicate;
	char fileName[FILE_NAME_SIZE];
	if (argc == 1)
		printf("No files were recieved for compilation!\n");
	else
	{
		for (i=1; i<=argc-1; i++)
		{
			strcpy(fileName, argv[i]);
			isDuplicate = 0;
			for (j=1; j<i; j++)
				if (strcmp(fileName,argv[j]) == 0)
					isDuplicate = 1;

			if (!isDuplicate)
			{
				if (validateFileName(fileName) != 0)
					printf("%s is not a valid assembly file name!\n", fileName);
				else
					compileFile(fileName);
			}

		}
	}


	return 0;
}



int validateFileName(char *fileName) /* ToDo */
{
	return 0;
}
void compileFile(char *fileName) /* ToDo */
{
	printf("'%s' was just theoreticallty compiled!\n", fileName);
}
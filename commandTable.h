#define COMMAND_NUM 27
#define DATA_COMMAND_LENGHT 8
#define DATA_COMMANDS 6

typedef struct s1
{
	char command[30];
	char action;
	int occation;
	int funct;
	int opcode;

}line;

typedef struct dataCommand
{
	char name[DATA_COMMAND_LENGHT];
	int occation;
	int bytes;

}dataCommand;

line lines[COMMAND_NUM];
dataCommand dataCommands[DATA_COMMANDS];
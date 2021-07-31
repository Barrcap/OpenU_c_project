#define COMMAND_NUM 27
#define DATA_COMMANDS 6

typedef struct s1
{

	char command[30];
	char action;
	int occation;
	int funct;
	int opcode;

}line;

typedef char dataCommand[DATA_COMMAND_LENGHT];


line lines[] = {
/*	command	action,case,opcode*/
	{"add",		'R'	,1	,1	,0},
	{"sub",		'R'	,1	,2	,0},
	{"and",		'R'	,1	,3	,0},
	{"or",		'R'	,1	,4	,0},
	{"nor",		'R'	,1	,5	,0},
	{"move",	'R'	,2	,1	,1},
	{"mvhi",	'R'	,2	,2	,1},
	{"mvlo",	'R'	,2	,3	,1},
	{"addi",	'I'	,3	,0	,10},
	{"subi",	'I'	,3	,0	,11},
	{"andi",	'I'	,3	,0	,12},
	{"ori",		'I'	,3	,0	,13},
	{"nori",	'I'	,3	,0	,14},
	{"bne",		'I'	,4	,0	,15},
	{"beq",		'I'	,4	,0	,16},
	{"blt",		'I'	,4	,0	,17},
	{"bgt",		'I'	,4	,0	,18},
	{"lb",		'I'	,3	,0	,19},
	{"sb",		'I'	,3	,0	,20},
	{"lw",		'I'	,3	,0	,21},
	{"sw",		'I'	,3	,0	,22},
	{"lh",		'I'	,3	,0	,23},
	{"sh",		'I'	,3	,0	,24},
	{"jmp",		'J'	,5	,0	,30},
	{"la",		'J'	,6	,0	,31},
	{"call",	'J'	,6	,0	,32},
	{"stop",	'J'	,7	,0	,63}
};

dataCommand dataCommands[] = 
{
	".dh",
	".dw",
	".db",
	".asciz",
	".entry",
	".extern"
};











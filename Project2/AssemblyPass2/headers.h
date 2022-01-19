#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*struct symbols {					// currently unused (symbol struct) -> might refactor code to implement struct symbol later 
	int	DefinedOnSourceLine;
	int	Address; 
	char Name[7]; 
};

typedef struct symbols SYMBOL;*/

/*struct opcodes {				   // currently unused (opcodes struct)
	char	OpCode;	
	char	Name[8];
};

typedef struct opcodes OPCODES;

OPCODES OpcodeTable[32];*/

int IsADirective(char *Test);
int IsAnInstruction(char *Test);

int compareMemory(int addressValue, int maxMemory);
int getFirstInstructionAddress(char *nextToken, char symbolNameList[1024][7], int symbolAddressList[1024], int symbolTableIndex);
int getSymbolAddress(char *thirdColumn, char symbolNameList[1024][7], int symbolAddressList[1024], int symbolTableIndex);

int ifByteC(char* nextToken);
int ifByteX(char *nextToken, int lineNum, FILE *fp);

void IsAValidSymbol(char *TestSymbol, char symbolNameList[1024][7], int symbolTableIndex, int lineNum, FILE *fp);
void checkWordLimit(char* nextToken, int wordIntegerConstant, int lineNum, FILE *fp);

char *getInstructionInfo(char *instructionName);

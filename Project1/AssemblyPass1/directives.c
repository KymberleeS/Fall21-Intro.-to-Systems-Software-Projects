#include "headers.h"

// function to check if a token is named after a directive
int IsADirective(char *Test){
	return(!(strcmp(Test, "START") && strcmp(Test, "END") &&
		 	 strcmp(Test, "BYTE") && strcmp(Test, "WORD") &&
		     strcmp(Test, "RESB") && strcmp(Test, "RESW") &&
		     strcmp(Test, "RESR") && strcmp(Test, "EXPORTS")));			 
}

// function to save the program's first executable instruction
int getFirstInstructionAddress(char *nextToken, char symbolNameList[150][7], int symbolAddressList[150], int symbolTableIndex) {
	int temp = 0;
	int value;

	// finds the index of the symbol specified by the END directive in the symbol table
	for (int i = 0; i < symbolTableIndex; i++) {
		if (strcmp(nextToken, symbolNameList[i]) == 0) {
			temp = i;
		}
	}

	// takes index i from the previous for loop to find the corresponding address value and saves it
	for (int i = 0; i < symbolTableIndex; i++) {
		if (temp == i) {
			value = symbolAddressList[i];
		}
	}

	return value;
}

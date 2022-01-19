#include "headers.h"

// function that ensures symbols meet the correct conditions
void IsAValidSymbol(char *TestSymbol, char symbolNameList[150][7], int symbolTableIndex, int lineNum, FILE *fp){
	int index = 0;
	int maxLength = 6;
	
	// while loop breaker
	int done = 0;

	// iterating through the symbols' characters
	while(done == 0) {
		// if character is null, while loop will break
		if (TestSymbol[index] == '\0') {
			break;
		}	

		// checking if symbol contains any invalid characters (referred to ascii)
		if ((TestSymbol[index] == 32) || (TestSymbol[index] == 36) || (TestSymbol[index] == 33) ||
			(TestSymbol[index] == 61) || (TestSymbol[index] == 43) || (TestSymbol[index] == 45) ||
			(TestSymbol[index] == 40) || (TestSymbol[index] == 41) || (TestSymbol[index] == 64)) {
				done = 1;

				printf("\nERROR (line %d): Symbol '%s' is invalid; symbol contains characters that are not allowed\n", lineNum, TestSymbol);
				fclose(fp);
				exit(0);
		}

		// checking if symbol exceeds max length allowed
		if (index >= maxLength) {
			done = 1;

			printf("\nERROR (line %d): Symbol '%s' is invalid; exceeds the maximum length allowed for a symbol\n", lineNum, TestSymbol);
			fclose(fp);
			exit(0);
		}
		index++;
	}

	// checking for duplicate symbol names by iterating through symbol table
	for (int i = 0; i < symbolTableIndex; i++) {
		if (strcmp(TestSymbol, symbolNameList[i]) == 0) {
			printf("\nERROR (line %d): Symbol '%s' is invalid; duplicate of name that already exists in the table\n", lineNum, TestSymbol);
			fclose(fp);
			exit(0);
		}
	}

	// checking if a symbol is named after a directive or an instruction
	if (IsADirective(TestSymbol)) {
		printf("\nERROR (line %d): Symbol '%s' named after existing directive\n", lineNum, TestSymbol);
		fclose(fp);
		exit(0);
	} else if (IsAnInstruction(TestSymbol)) {
		printf("\nERROR (line %d): Symbol '%s' named after existing instruction\n", lineNum, TestSymbol);
		fclose(fp);
		exit(0);
	}
}

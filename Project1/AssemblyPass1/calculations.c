#include "headers.h"

// function for max memory error handling; compares current address to the max memory of SIC
int compareMemory(int addressValue, int maxMemory) {
	if (addressValue >= maxMemory) {
		return 1;
	} 
	return 0;
}

// if BYTE flag is C, string length is recorded to be added to the location counter
int ifByteC(char* nextToken) {
    nextToken = strtok(NULL, "'");
    return strlen(nextToken);
}

// if BYTE flag is X, returns length of the token; in main.c, length is multiplied by 0.5, since hexadecimal digit = 4 bits (half byte)
int ifByteX(char *nextToken, int lineNum, FILE *fp) {
    nextToken = strtok(NULL, "'");

	// checks for invalid hex constant (if contains any letters past F)
    for (int i = 0; i < strlen(nextToken); i++) {
	    if(!((nextToken[i] >= 0) && (nextToken[i] <= 70))) {
		    printf("\nERROR (line %d): Invalid hex constant for directive: BYTE\n", lineNum);
			fclose(fp);
			exit(0);
		}
    }
    return strlen(nextToken);
}

// function to check if current word integer constant is between -2^23 and 2^23 (accomodates for signed integers, according to SIC specifications)
void checkWordLimit(char* nextToken, int wordIntegerConstant, int lineNum, FILE *fp) {
    if ((atoi(nextToken) < (-1 * wordIntegerConstant)) || (atoi(nextToken) > wordIntegerConstant)) {
		printf("\nERROR (line %d): Current WORD constant exceeds the 24-bit limit\n", lineNum);
		fclose(fp);
		exit(0);
	}
}

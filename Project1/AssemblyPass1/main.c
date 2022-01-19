#include "headers.h"

int main(int argc, char* argv[]) {
	// file pointer: a pointer to a structure that represents a file
	FILE *fp;

	// variables
	char fileLine[1024];
	/*char fullLine[1024];*/								// currently unused
	char* newSymbol;
	char* nextToken;

	int locationCounter; 
	int address;

	// symbol table to store symbols and their addresses (implemented using arrays)
	char symbolNameList[1024][7];
	int symbolAddressList[1024];
	int symbolTableIndex = 0; 

	// variable that stores the address of the first executable instruction of the program
	int firstInstructionAddress;
		
	// file reading and file reading error handling
	if (argc != 2) {
		printf("ERROR: Usage: %s filename\n", argv[0]);
		return 0;
	}
	fp = fopen(argv[1], "r");

	if (fp == NULL) {
		printf("ERROR: file '%s' does not exist or could not be opened for reading\n", argv[1]);
		return 0;
	}

	// allocates memory - system dependent - char (int, etc.) size differs per system
	newSymbol = malloc(1024 * sizeof(char));	
	memset(newSymbol, '\0', 1024 * sizeof(char));

	nextToken = malloc(1024 * sizeof(char));	
	memset(nextToken, '\0', 1024 * sizeof(char));

	// initialize line number for the program file
	int lineNum = 1;

	// initialize maximum memory based on SIC specification
	int maxMemory = 32768;

	// initialize limit of the integer constant for directive WORD (-8388608 <= integer constant <= 8388608; 2^23 bits due to two's complement)
	int wordIntegerConstant = 8388608;

	// reading through the file to get the output table
	while(fgets(fileLine, 1024, fp) != NULL) {
		/*strcpy(fullLine, fileLine);*/					// currently unused				

		// if line is a comment, ignore (35 == ascii code for #)
		if (fileLine[0] == 35) {  
			lineNum++;
			continue;
		}

		// initial symbol handling - checking if line starts with [A-Z]
		if ((fileLine[0] >= 65) && (fileLine[0] <= 90)) {
			// parses line to get symbol and next token
			newSymbol = strtok(fileLine, "\t\n");
			nextToken = strtok(NULL, " \t\n");

			// symbol error handling (see symbols.c for more details on function)
			IsAValidSymbol(newSymbol, symbolNameList, symbolTableIndex, lineNum, fp);

			// initializing the location counter/address set up
			if (strcmp(nextToken, "START") == 0) {
				char* startAddress = strtok(NULL, "\t\n");
				sscanf(startAddress, "%x", &locationCounter);

				// if the address STARTS max memory
				if (compareMemory(locationCounter, maxMemory) == 1) {
					printf("\nERROR (line %d): Unable to execute program; start address set to SIC memory's maximum capacity\n", lineNum);
					fclose(fp);
					return 0;
				}

				// printing very first symbol
				printf("%s\t%04X\n", newSymbol, locationCounter); 

				// storing locationCounter to the address (handles the offset)
				address = locationCounter;
				lineNum++;

				// storing first symbol and its address to the symbol table
				strcpy(symbolNameList[symbolTableIndex], newSymbol);
				symbolAddressList[symbolTableIndex] = address;
				symbolTableIndex++;

				continue;
			}

			// printing the symbols and their addresses
			printf("%s\t%04X\n", newSymbol, address);

			// storing the symbols and their address to the symbol table
			strcpy(symbolNameList[symbolTableIndex], newSymbol);
			symbolAddressList[symbolTableIndex] = address;
			symbolTableIndex++;
		} else if (isspace(*newSymbol)) {
			// if spaces occupy symbol spot, takes the tokenizes the next work on the file line
			nextToken = strtok(fileLine, "\t\r");
		} else {
			// error if the file line begins with invalid characters
			newSymbol = strtok(fileLine, "\t\n");

			printf("\nERROR (line %d): Symbol '%s' is invalid; symbols only begin with characters [A-Z]\n", lineNum, newSymbol);
			fclose(fp);
			return 0;
		}

		// instruction and directive conditions/error handling 
		if (IsADirective(nextToken)) {
			if (strcmp(nextToken, "START") == 0) {
				// if directive is START, nothing happens to location counter (was already set at the beginning of the program)
				locationCounter += 0;
			} else if (strcmp(nextToken, "END") == 0) {
				// if directive is END, record address of program's first executable instruction
				nextToken = strtok(NULL, "\t\t");
				
				// check directives.c for more details on function
				firstInstructionAddress = getFirstInstructionAddress(nextToken, symbolNameList, symbolAddressList, symbolTableIndex);

				// nothing happens to the location counter
				locationCounter += 0;
			} else if (strcmp(nextToken, "BYTE") == 0) {
				nextToken = strtok(NULL, " '");
				
				if (strcmp(nextToken, "C") == 0) {
					// check calculations.c for more details on function
					locationCounter += ifByteC(nextToken);
				} else if (strcmp(nextToken, "X") == 0) {
					// single hexadecimal digit = 4 bits = half a byte (nibble); check calculations.c for more details on function
					locationCounter += 0.5 * ifByteX(nextToken, lineNum, fp);
				} else {
					// error if flags other than C or X appear, or formatting is incorrect in general
					printf("\nERROR (line %d): Invalid operand for directive: BYTE\n", lineNum);
					fclose(fp);
					return 0;
				}
			} else if (strcmp(nextToken, "WORD") == 0) {
				nextToken = strtok(NULL, "\t\t");

				// check calculations.c for more details on function
				checkWordLimit(nextToken, wordIntegerConstant, lineNum, fp);
				locationCounter += 3;
			} else if (strcmp(nextToken, "RESB") == 0) {
				// adds number of bytes from RESB to location counter
				char* RESBAddress = strtok(NULL, "\t\n");
				locationCounter += atoi(RESBAddress);
			} else if (strcmp(nextToken, "RESW") == 0) {
				// adds number of bytes from RESW times 3 to location counter
				char* RESWAddress = strtok(NULL, "\t\n");
				locationCounter += (3 * atoi(RESWAddress));
			} else if (strcmp(nextToken, "RESR") == 0) {
				// adds 3 bytes to location counter
				locationCounter += 3;
			} else if (strcmp(nextToken, "EXPORTS") == 0) {
				// adds 3 bytes to location counter
				locationCounter += 3;
			} 
		} else if (IsAnInstruction(nextToken)) {
			// if any instruction, adds 3 bytes to location counter
			locationCounter += 3;
		} else if (isspace(*nextToken)) {
			// error if spaces occupy directive/instruction spots; line is blank
			printf("\nERROR (line %d): Blank or empty lines; violation of SIC specification\n", lineNum);
			fclose(fp);
			return 0;
		} else {
			// error if directive or instruction does not exist
			printf("\nERROR (line %d): Invalid directive or instruction\n", lineNum);
			fclose(fp);
			return 0;
		}
		lineNum++;

		// updates the address every time a line is read/analyzed
		address = locationCounter;

		// if the address exceeds max memory in general; check calculations.c for more details on function
		if (compareMemory(address, maxMemory) == 1) {
			printf("\nERROR (line %d): Maximum memory for SIC has been exceeded\n", lineNum);
			fclose(fp);
			return 0;
		} 
	}

	// notifies that the first instruction address was saved
	printf("\nNOTE: Address of the program's first executable instruction has been recorded: %04X\n\n", firstInstructionAddress);

	fclose(fp);
	return 0;
}

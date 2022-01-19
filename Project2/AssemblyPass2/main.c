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
	int modificationArray[1024];
	int symbolTableIndex = 0; 

	// object file variables (PASS 2)
	int objectFileIndex = 0;
	int modificationIndex = 0;
	int locationCounterArray[1024];
	int thirdColumnArrayAddress[1024];
	int numOfBytes[1024];

	char opcodeArray[1024][3];
	char thirdColumnArray[1024][10];

	char* endToken = ""; 
	endToken = malloc(1024 * sizeof(char));	
	memset(endToken, '\0', 1024 * sizeof(char));

	char* thirdColumn = ""; 
	endToken = malloc(1024 * sizeof(char));	
	memset(endToken, '\0', 1024 * sizeof(char));

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

	// start line variable to keep track of START directive existence
	int startLineNum = 1;

	// initialize maximum memory based on SIC specification
	int maxMemory = 32768;

	// initialize limit of the integer constant for directive WORD (-8388608 <= integer constant <= 8388608; 2^23 bits due to two's complement)
	int wordIntegerConstant = 8388608;

	// stores start address for header in PASS 2
	int storeStartAddress = 0;

	// reading through the file to get the output table
	while(fgets(fileLine, 1024, fp) != NULL) {
		/*strcpy(fullLine, fileLine);*/					// currently unused				

		// if line is a comment, ignore (35 == ascii code for #)
		if (fileLine[0] == 35) {  
			lineNum++;
			startLineNum++;
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

				// to keep track of start address for header in PASS 2
				storeStartAddress = locationCounter;

				// if the address STARTS max memory
				if (compareMemory(locationCounter, maxMemory) == 1) {
					printf("\nERROR (line %d): Unable to execute program; start address set to SIC memory's maximum capacity\n", lineNum);
					fclose(fp);
					return 0;
				}

				// printing very first symbol
				//printf("%s\t%04X\n", newSymbol, locationCounter); 				// currently unused

				// storing locationCounter to the address (handles the offset)
				address = locationCounter;
				lineNum++;

				// storing first symbol and its address to the symbol table
				strcpy(symbolNameList[symbolTableIndex], newSymbol);
				symbolAddressList[symbolTableIndex] = address;
				symbolTableIndex++;

				//continue;
			} else if (!(lineNum > startLineNum)) {
				// if the current token is not START AND if the current line number is not greater than START's line number, START doesn't exist
				printf("\nERROR (line %d): Unable to execute program; START directive not present\n", lineNum);
				fclose(fp);
				return 0;
			}
			// printing the symbols and their addresses
			//printf("%s\t%04X\n", newSymbol, address);								// currently unused

			// storing the symbols and their address to the symbol table
			strcpy(symbolNameList[symbolTableIndex], newSymbol);
			symbolAddressList[symbolTableIndex] = address;
			symbolTableIndex++;
		} else if (isspace(*newSymbol)) {
			// if spaces occupy symbol spot, takes the tokenizes the next work on the file line
			nextToken = strtok(fileLine, "\t\r ");
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
				nextToken = strtok(NULL, "\t\n\r");

				// recording end token for verification for object file output
				strcpy(endToken, nextToken);
				
				// check directives.c for more details on function
				firstInstructionAddress = getFirstInstructionAddress(nextToken, symbolNameList, symbolAddressList, symbolTableIndex);

				// nothing happens to the location counter
				locationCounter += 0;
			} else if (strcmp(nextToken, "BYTE") == 0) {
				nextToken = strtok(NULL, " '");
				
				if (strcmp(nextToken, "C") == 0) {
					// filling in the 3rd column with a spacer since it won't be a symbol
					strcpy(thirdColumnArray[objectFileIndex], "\r");

					// setting the "opcode" as 00 since directives don't have opcodes
					strcpy(opcodeArray[objectFileIndex], "00");

					// keeping track of location counter for T record
					locationCounterArray[objectFileIndex] = locationCounter;
				
					// check calculations.c for more details on function
					int temp = ifByteC(nextToken);
					locationCounter += temp;

					numOfBytes[objectFileIndex] = temp;
					thirdColumnArrayAddress[objectFileIndex] = numOfBytes[objectFileIndex];
					objectFileIndex++;
				} else if (strcmp(nextToken, "X") == 0) {
					// filling in the 3rd column with a spacer since it won't be a symbol
					strcpy(thirdColumnArray[objectFileIndex], "\r");

					// setting the "opcode" as 00 since directives don't have opcodes
					strcpy(opcodeArray[objectFileIndex], "00");

					// keeping track of location counter for T record
					locationCounterArray[objectFileIndex] = locationCounter;

					// single hexadecimal digit = 4 bits = half a byte (nibble); check calculations.c for more details on function
					int temp = 0.5 * ifByteX(nextToken, lineNum, fp);
					locationCounter += temp;//0.5 * ifByteX(nextToken, lineNum, fp);

					numOfBytes[objectFileIndex] = temp;
					thirdColumnArrayAddress[objectFileIndex] = numOfBytes[objectFileIndex];
					objectFileIndex++;

				} else {
					// error if flags other than C or X appear, or formatting is incorrect in general
					printf("\nERROR (line %d): Invalid operand for directive: BYTE\n", lineNum);
					fclose(fp);
					return 0;
				}
			} else if (strcmp(nextToken, "WORD") == 0) {
				nextToken = strtok(NULL, "\t\r");

				// check calculations.c for more details on function
				checkWordLimit(nextToken, wordIntegerConstant, lineNum, fp);

				// filling in the 3rd column with a spacer since it won't be a symbol
				strcpy(thirdColumnArray[objectFileIndex], "\r");

				// setting the "opcode" as 00 since directives don't have opcodes
				strcpy(opcodeArray[objectFileIndex], "00");

				// keeping track of location counter for T record
				locationCounterArray[objectFileIndex] = locationCounter;
				numOfBytes[objectFileIndex] = 3;

				// keeping track of the length (in hex) of the 3rd column since not a symbol
				thirdColumnArrayAddress[objectFileIndex] = atoi(nextToken);
			
				objectFileIndex++;

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
			// recording addresses of 3rd column for object file output; will work hand in hand with symbol table
			thirdColumn = strtok(NULL, "\t\n ");
			strcpy(thirdColumnArray[objectFileIndex], thirdColumn);

			// recording opcodes for object file output
			strcpy(opcodeArray[objectFileIndex], getInstructionInfo(nextToken));

			// recording current location counter for every line for object file output
			locationCounterArray[objectFileIndex] = locationCounter;
			modificationArray[modificationIndex] = locationCounter + 1; 
			numOfBytes[objectFileIndex] = 3;
			
			objectFileIndex++;
			modificationIndex++;

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
	//printf("\nNOTE: Address of the program's first executable instruction has been recorded: %04X\n\n", firstInstructionAddress); 	// currently unused


	// checking if the end symbol is valid for the program's first executable instruction. If not, and error occurs and the program stops
	for (int i = 0; i < symbolTableIndex; i++) {
		if (strcmp(endToken, symbolNameList[i]) == 0) {
			break;
		} else if ((i == symbolTableIndex - 1) && (strcmp(endToken, symbolNameList[i]) != 0)) {	
			printf("\nERROR: Symbol %s not found in the symbol table\n", endToken);
			fclose(fp);
			exit(0);
		}
	}

	// checking to make sure that a symbol (in 3rd column) before the ',X' substring is valid with a BYTE directive 
	for (int i = 0; i < objectFileIndex; i++) {
		for (int j = 0; j < symbolTableIndex; j++) {
			if (strcmp(thirdColumnArray[i], symbolNameList[j]) == 0) {
				thirdColumnArrayAddress[i] = getSymbolAddress(thirdColumnArray[i], symbolNameList, symbolAddressList, symbolTableIndex);
				break;
			} else if ((strstr(thirdColumnArray[i], ",X")) && (strstr(thirdColumnArray[i], symbolNameList[j]))) {
				if (strcmp(strtok(thirdColumnArray[i], ","), symbolNameList[j]) == 0) {
					thirdColumnArrayAddress[i] = getSymbolAddress(symbolNameList[j], symbolNameList, symbolAddressList, symbolTableIndex) + 32768;
				} else {
					printf("\nERROR: Symbol %s not found in the symbol table\n", thirdColumnArray[i]);
					fclose(fp);
					exit(0);
				}
				i++;
			} else if ((j == symbolTableIndex - 1) && (strcmp(thirdColumnArray[i], symbolNameList[j]) != 0)) {
				if (strstr(thirdColumnArray[i], "\r")) {			
					;
				} else {
					printf("\nERROR: Symbol %s not found in the symbol table\n", thirdColumnArray[i]);
					fclose(fp);
					exit(0);
				}
			} 
		}
	}
	
	// creating object file and its contents IF no errors occur (the program should AND will stop if an error happens)
	char objectFileName[0x100];
	snprintf(objectFileName, sizeof(objectFileName), "%s.obj", argv[1]);

	fp = fopen(objectFileName, "w");
   	fprintf(fp, "H%s\t00%04X00%04X\n", symbolNameList[0], storeStartAddress, ((locationCounter + 3) - storeStartAddress));

	for (int i = 0; i < objectFileIndex; i++) {
		fprintf(fp, "T00%04X%02X%s%04X\n", locationCounterArray[i], numOfBytes[i], opcodeArray[i], thirdColumnArrayAddress[i]);
	}

	for (int i = 0; i < modificationIndex; i++) {
		fprintf(fp, "M00%04X04+%s\n", modificationArray[i], symbolNameList[0]);
	}
 
	fprintf(fp, "E00%04X\n", firstInstructionAddress);
   	
	fclose(fp);
	return 0;
} 

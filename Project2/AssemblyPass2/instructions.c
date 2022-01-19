#include "headers.h"

// function to check if a token is named after an instruction
int IsAnInstruction(char *Test) {
	return(!(strcmp(Test, "ADD") && strcmp(Test, "ADDF") && strcmp(Test, "ADDR") && 
	       strcmp(Test, "AND") && strcmp(Test, "CLEAR") && strcmp(Test, "COMP") && 
		   strcmp(Test, "COMPF") && strcmp(Test, "COMPR") && strcmp(Test, "DIV") &&
		   strcmp(Test, "DIVF") && strcmp(Test, "DIVR") && strcmp(Test, "FIX") &&
		   strcmp(Test, "FLOAT") && strcmp(Test, "HIO") && strcmp(Test, "J") &&
		   strcmp(Test, "JEQ") && strcmp(Test, "JGT") && strcmp(Test, "JLT") &&
		   strcmp(Test, "JSUB") && strcmp(Test, "LDA") && strcmp(Test, "LDB") &&
		   strcmp(Test, "LDCH") && strcmp(Test, "LDF") && strcmp(Test, "LDL") &&
		   strcmp(Test, "LDS") && strcmp(Test, "LDT") && strcmp(Test, "LDX") &&
		   strcmp(Test, "LPS") && strcmp(Test, "MUL") && strcmp(Test, "MULF") &&
		   strcmp(Test, "MULR") && strcmp(Test, "NORM") && strcmp(Test, "OR") &&
		   strcmp(Test, "RD") && strcmp(Test, "RMO") && strcmp(Test, "RSUB") &&
		   strcmp(Test, "SHIFTL") && strcmp(Test, "SHIFTR") && strcmp(Test, "SIO") &&
		   strcmp(Test, "SSK") && strcmp(Test, "STA") && strcmp(Test, "STB") &&
		   strcmp(Test, "STCH") && strcmp(Test, "STF") && strcmp(Test, "STI") && 
		   strcmp(Test, "STL") && strcmp(Test, "STS") && strcmp(Test, "STSW") &&
		   strcmp(Test, "STT") && strcmp(Test, "STX") && strcmp(Test, "SUB") &&
		   strcmp(Test, "SUBF") && strcmp(Test, "SUBR") && strcmp(Test, "SVC") && 
		   strcmp(Test, "TD") && strcmp(Test, "TIO") && strcmp(Test, "TIX") &&
		   strcmp(Test, "TIXR") && strcmp(Test, "WD")));
}

char *getInstructionInfo(char *instructionName) {
	char instructionNameList[1024][6] = {"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
		   								 "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", "LDS", 
										 "LDT", "LDX", "LPS", "MUL", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", 
										 "SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW", "STT", "STX", "SUB", "SUBF", 
										 "SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD"};
	
	char instructionOpcodeList[1024][3] = {"18", "58", "90", "40", "B4", "28", "88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C", "30", "34", "38",
										   "48", "00", "68", "50", "70", "08", "6C", "74", "04", "D0", "20", "60", "98", "C8", "44", "D8", "AC", "4C",
										   "A4", "A8", "FO", "EC", "OC", "78", "54", "80", "D4", "14", "7C", "E8", "84", "10", "1C", "5C", "94", "B0",
										   "E0", "F8", "2C", "B8", "DC"};
			

	int temp = 0;
	char *value = " ";

	value = malloc(1024 * sizeof(char));	
	memset(value, '\0', 1024 * sizeof(char));

	for (int i = 0; i < 1024; i++) {
		if (strcmp(instructionName, instructionNameList[i]) == 0) {
			temp = i;
		}
	}

	for (int i = 0; i < 1024; i++) {
		if (temp == i) {
			strcpy(value, instructionOpcodeList[i]);
		}
	} 
 
	return value;
}

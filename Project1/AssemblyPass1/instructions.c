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

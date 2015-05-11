#ifndef CPU_H
#define CPU_H

#include "globals.h"

#define STACK_SIZE 256

#define ip (registers[IP])
#define sp (registers[SP])

typedef enum { 
	PSH,
	ADD,
	POP,
	SET,
	MOV,
	LOG,
	JMP,
	HLT,
	NUM_INSTRUCTIONS
} InstructionSet;

typedef enum {
	A, B, C, D, E, F, IP, SP,
	NUM_REGISTERS
} Registers;

bool running;

int stack[STACK_SIZE];

int registers[NUM_REGISTERS];

int *program;

void initCPU();
void runProgram();

#endif

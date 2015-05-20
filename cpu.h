#ifndef CPU_H
#define CPU_H

#include "globals.h"

#define STACK_SIZE 256

#define ip (registers[IP])
#define sp (registers[SP])
#define lr (registers[LR])

typedef enum { 
	PSH,PSHR,
	ADD,ADDI,
	SUB,SUBI,
	MUL,MULI,
	DIV,DIVI,
	POP,POPR,
	SET,
	MOV,
	SRL,SRR,
	LOG,
	PUTC,PUTD,PUTS,
	JMP,JNZ,JLT,JGT,JRE,
	LDA,
	HLT,
	NUM_INSTRUCTIONS
} InstructionSet;

typedef enum {
	A, B, C, D, E, F,
	S0, S1, S2,
	IP, SP, LR,
	NUM_REGISTERS
} Registers;

bool running;

int stack[STACK_SIZE];

int registers[NUM_REGISTERS];

int *program;

DataEntry *dataSection[100];
int dataCount;

void initCPU();
void runProgram();

#endif

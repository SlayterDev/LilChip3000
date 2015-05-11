#ifndef CPU_H
#define CPU_H

#define STACK_SIZE 256

#define ip (registers[IP])
#define sp (registers[SP])

typedef enum { 
	PSH,
	ADD,
	POP,
	SET,
	HLT,
	NUM_INSTRUCTIONS
} InstructionSet;

typedef enum {
	A, B, C, D, E, F, IP, SP,
	NUM_REGISTERS
} Registers;

char regNames[][2] = {"A", "B", "C", "D", "E", "F", "IP", "SP"};

bool running;

int stack[STACK_SIZE];

int registers[NUM_REGISTERS];

#endif
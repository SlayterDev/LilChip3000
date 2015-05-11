#include "cpu.h"

char regNames[][2] = {"A", "B", "C", "D", "E", "F", "IP", "SP"};

int program[] = {
	PSH, 5,
	PSH, 6,
	ADD,
	POP,
	SET, A, 10,
	MOV, B, A,
	LOG, B,
	HLT
};

int fetch() {
	return program[ip];
}

void eval(int instr) {
	switch (instr) {
		case HLT:
			running = false;
			break;
		case PSH:
			stack[++sp] = program[++ip];
			break;
		case POP: {
			int val = stack[sp--];
			printf("Popped: %d\n", val);
			break;
		}
		case ADD: {
			int a = stack[sp--];
			int b = stack[sp--];
			stack[++sp] = a + b;
			break;
		}
		case SET: {
			int reg = program[++ip];
			registers[reg] = program[++ip];;

			printf("Reg %s: %d\n", regNames[reg], registers[reg]);
			break;
		}
		case MOV: {
			int dest = program[++ip];
			int src = program[++ip];
			registers[dest] = registers[src];
			break;
		}
		case LOG: {
			int reg = program[++ip];
			printf("Reg %s: %d\n", regNames[reg], registers[reg]);
			break;
		}
		default:
			break;
	}
}

void initCPU() {
	ip = 0;
	sp = -1;
	running = true;
}

void runProgram() {
	while (running) {
		eval(fetch());
		ip++;
	}
}
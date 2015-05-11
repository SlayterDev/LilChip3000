#include "cpu.h"
#include "fileOps.h"

char regNames[][2] = {"A", "B", "C", "D", "E", "F", "IP", "SP"};

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
		case PSHR: {
			int reg = program[++ip];
			stack[++sp] = registers[reg];
			break;
		}
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
		case PUTC: {
			int reg = program[++ip];
			printf("%c", registers[reg]);
			break;
		}
		case JMP: {
			ip = program[++ip] - 1; // Subtract 1 because the ip 
			break;					// increments after this
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

void runProgram(char *filename) {
	program = readProgram(filename);

	printf("Starting Program...\n\n");
	while (running) {
		eval(fetch());
		ip++;
	}

	free(program);
}

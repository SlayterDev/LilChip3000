#include "cpu.h"
#include "fileOps.h"

char regNames[][2] = {"A", "B", "C", "D", "E", "F", "IP", "SP"};

/* 
 Set the lr to the instruction after the current instruction
 offset should be equal to the number of arguments in the 
 current function
*/
void setlr(int offset) {
	lr = ip + offset;
}

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
		case POPR: {
			int val = stack[sp--];
			int reg = program[++ip];
			registers[reg] = val;
			break;
		}
		case ADD: {
			int a = stack[sp--];
			int b = stack[sp--];
			stack[++sp] = a + b;
			break;
		}
		case ADDI: {
			int reg = program[++ip];
			registers[reg] += program[++ip];
			break;
		}
		case SUB: {
			int a = stack[sp--];
			int b = stack[sp--];
			stack[++sp] = a - b;
			break;
		}
		case SUBI: {
			int reg = program[++ip];
			registers[reg] -= program[++ip];
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
		case PUTD: {
			int reg = program[++ip];
			printf("%d", registers[reg]);
			break;
		}
		case JMP: {
			setlr(1);
			ip = program[++ip] - 1; // Subtract 1 because the ip  
			break;					// increments after this
		}
		case JNZ: {
			setlr(2);
			int reg = program[++ip];
			ip++;
			if (registers[reg] > 0) {
				ip = program[ip] - 1;
			}
			break;
		}
		case JLT: {
			setlr(3);
			int reg1 = program[++ip];
			int reg2 = program[++ip];
			ip++;
			if (registers[reg1] < registers[reg2]) {
				ip = program[ip] - 1;
			}
			break;
		}
		case JGT: {
			setlr(3);
			int reg1 = program[++ip];
			int reg2 = program[++ip];
			ip++;
			if (registers[reg1] > registers[reg2]) {
				ip = program[ip] - 1;
			}
			break;
		}
		case JRE: {
			setlr(3);
			int reg1 = program[++ip];
			int reg2 = program[++ip];
			ip++;
			if (registers[reg1] == registers[reg2]) {
				ip = program[ip] - 1;
			}
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

void runProgram(char *filename) {
	program = readProgram(filename);

	printf("Starting Program...\n\n");
	while (running) {
		eval(fetch());
		ip++;
	}

	free(program);
}

#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"

int program[] = {
	PSH, 5,
	PSH, 6,
	ADD,
	POP,
	SET, A, 10,
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
		default:
			break;
	}
}

int main(int argc, char const *argv[]) {
	
	ip = 0;
	sp = -1;
	running = true;

	while (running) {
		eval(fetch());
		ip++;
	}
	

	return 0;
}
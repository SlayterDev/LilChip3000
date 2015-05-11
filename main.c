#include "globals.h"
#include "cpu.h"

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <chipFile>\n", argv[0]);
		exit(1);
	}

	initCPU();
	runProgram(argv[1]);

	return 0;
}

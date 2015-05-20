#include "fileOps.h"
#include "datasection.h"

int *readProgram(char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Could not open program %s\n", filename);
		exit(1);
	}

	if (!parseDataSection(f)) {
		rewind(f);
	}


	int pos = ftell(f);

	int input, count = 0;
	while (fscanf(f, "%d", &input) != EOF)
		count++;

	int *code = (int *)malloc(sizeof(int)*count);
	fseek(f, pos, SEEK_SET);
	count = 0;
	while (fscanf(f, "%d", &input) != EOF) {
		code[count] = input;
		count++;
	}

	fclose(f);
	return code;
}

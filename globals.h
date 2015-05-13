#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	INT, STRING
} types;

typedef struct {
	char *name;
	types type;
	void *data;
	int numElements;
} DataEntry;

#endif

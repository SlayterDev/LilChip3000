#include "datasection.h"
#include "cpu.h"

void *getIntType(char *buffer, DataEntry *entry) {
	int *data;
	int elemCount = 0;
	if (buffer[0] != '{') {
		data = (int *)malloc(sizeof(int));
		data[0] = atoi(buffer);
		elemCount++;
	} else {
		for (int i = 0; i < strlen(buffer); i++) {
			if (buffer[i] == ',')
				elemCount++;
		}
		elemCount++;

		// trim the brackets
		buffer = buffer + 1;
		buffer[strlen(buffer)-1] = '\0';

		data = (int *)malloc(sizeof(int)*elemCount);
		char *tok = strtok(buffer, ",");
		data[0] = atoi(tok);

		for (int i = 1; i < elemCount; i++) {
			tok = strtok(NULL, ",");
			data[i] = atoi(tok);
		}
	}

	entry->elementCount = elemCount;

	return (void *)data;
}

void *getStringType(char *buffer, DataEntry *entry) {
	// Trim the quotes
	buffer = buffer + 1;
	buffer[strlen(buffer)-2] = '\0';

	char *data = (char *)malloc(strlen(buffer)+1);
	strcpy((char *)data, buffer);

	entry->elementCount = 1;

	return (void *)data;
}

void dumpDataSection() {
	for (int i = 0; i < dataCount; i++) {
		switch (dataSection[i]->type) {
			case STRING:
				printf("Data %d: %s %s\n", i, dataSection[i]->name, (char *)dataSection[i]->data);
				break;
		}
	}
}

void parseDataSection(FILE *f) {
	char buffer[1024];
	fgets(buffer, 1024, f);

	// Force loop to check for opening block
	// strcmp() does not work here for whatever reason
	bool match = true;
	for (int i = 0; i < 4; i++) {
		if (buffer[i] != '=') {
			match = false;
			break;
		}
	}
	if (!match)
		return;

	do {
		memset(buffer, 0, 1024); // clear the buffer
		fgets(buffer, 1024, f);  // grab the next line
		buffer[strlen(buffer)-1] = '\0';

		// Force loop to check for closing block
		// strcmp() does not work here for whatever reason
		bool match = true;
		for (int i = 0; i < 4; i++) {
			if (buffer[i] != '=') {
				match = false;
				break;
			}
		}
		if (match)
			break;

		char *tempBuffer = (char *)malloc(strlen(buffer)+1);
		strcpy(tempBuffer, buffer);

		char *tok = strtok(tempBuffer, " "); // tok has type
		int dataType = atoi(tok);

		tok = strtok(NULL, " "); // tok has name
		char *name = (char *)malloc(strlen(tok)+1);
		strcpy(name, tok);

		tok = strtok(NULL, " "); // tok has start of data
		int offset = tok - tempBuffer;
		tok = buffer + offset;

		void *data;
		DataEntry *entry = (DataEntry *)malloc(sizeof(DataEntry));

		switch (dataType) {
			case INT:
				data = getIntType(tok, entry);
				break;
			case STRING:
				data = getStringType(tok, entry);
				break;
			default:
				break;
		}

		entry->name = name;
		entry->type = dataType;
		entry->data = data;

		dataSection[dataCount++] = entry;

		free(tempBuffer);

	} while (strcmp(buffer, "===="));
	printf("End of loop!\n");

	dumpDataSection();
}

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

	entry->numElements = elemCount;

	return (void *)data;
}

// Because the compiler gives us the escape characters in the string,
// We need to manually put them into the buffer
void processEscapeChars(char *buffer) {
	for (int i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == '\\') {
			switch (buffer[i+1]) {
				case 'n':
					buffer[i] = '\n';
					break;
				case '\"':
					buffer[i] = '\"';
					break;
				case '\'':
					buffer[i] = '\'';
					break;
				case '\\':
					buffer[i] = '\\';
					break;
				case 't':
					buffer[i] = '\t';
					break;
				case 'r':
					buffer[i] = '\r';
					break;
				default:
					break;
			}

			// shift the rest of the string
			for (int j = i+1; j < strlen(buffer); j++) {
				buffer[j] = buffer[j+1];
			}
		}
	}
}

void *getStringType(char *buffer, DataEntry *entry) {
	// Trim the quotes
	buffer = buffer + 1;
	buffer[strlen(buffer)-1] = '\0';

	// Windows line ending fix (ex. "Some Text\r\n")
	if (buffer[strlen(buffer)-1] == '\r' || buffer[strlen(buffer)-1] == '\n')
		buffer[strlen(buffer)-1] = '\0';

	processEscapeChars(buffer);

	char *data = (char *)malloc(strlen(buffer)+1);
	strcpy((char *)data, buffer);

	entry->numElements = 1;

	return (void *)data;
}

void dumpDataSection() {
	for (int i = 0; i < dataCount; i++) {
		switch (dataSection[i]->type) {
			case STRING:
				printf("Data %d: %s %s\n", i, dataSection[i]->name, (char *)dataSection[i]->data);
				break;
			case INT:
				printf("Data %d: %s {", i, dataSection[i]->name);

				int count = dataSection[i]->numElements;
				int *array = (int *)dataSection[i]->data;
				for (int j = 0; j < count; j++) {
					printf("%d", array[j]);

					if (j < count-1)
						printf(", ");
					else
						printf("}\n");
				}

				break;
		}
	}
}

bool parseDataSection(FILE *f) {
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
	if (!match) {
		return false;
	}

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
	
	//dumpDataSection();

	return true;
}

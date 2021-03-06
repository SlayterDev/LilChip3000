#!/usr/bin/python

import os
import sys

instructions = ["PSH", "PSHR", "ADD", "ADDI", "SUB", "SUBI", "MUL", "MULI", "DIV", "DIVI",
				"POP", "POPR", 
				"SET", "MOV", "SRL", "SRR",
				"LOG", "PUTC", "PUTD", "PUTS", 
				"JMP", "JNZ", "JLT", "JGT", "JRE", "LDA", "HLT"]
registers = ["A", "B", "C", "D", "E", "F", "S0", "S1", "S2", "IP", "SP", "LR"]

instructionInfo = {
"PSH":  {"argc": 1,"regs": False},
"PSHR": {"argc": 1,"regs": True},
"ADD":  {"argc": 0,"regs": False},
"ADDI": {"argc": 2,"regs": True},
"SUB":  {"argc": 0,"regs": False},
"SUBI": {"argc": 2,"regs": True},
"MUL":  {"argc": 0,"regs": False},
"MULI": {"argc": 2,"regs": True},
"DIV":  {"argc": 0,"regs": False},
"DIVI": {"argc": 2,"regs": True},
"POP":  {"argc": 0,"regs": False},
"POPR": {"argc": 1,"regs": True},	
"SET":  {"argc": 2,"regs": True},
"MOV":  {"argc": 2,"regs": True},
"SRL":  {"argc": 2,"regs": True},
"SRR":  {"argc": 2,"regs": True},
"LOG":  {"argc": 1,"regs": True},
"PUTC": {"argc": 1,"regs": True},
"PUTD": {"argc": 1,"regs": True},
"PUTS": {"argc": 1,"regs": True},
"JMP":  {"argc": 1,"regs": False},
"JNZ":  {"argc": 2,"regs": True},
"JLT":  {"argc": 3,"regs": True},
"JGT":  {"argc": 3,"regs": True},
"JRE":  {"argc": 3,"regs": True},
"LDA":  {"argc": 2,"regs": True},
"HLT":  {"argc": 0,"regs": False},
}

mixedInstructions = ["SET", "JNZ", "ADDI", "SUBI", "MULI", "DIVI", "SRL", "SRR"]
tripleInstructions = ["JLT", "JGT", "JRE"]

dataTypes = ["INT", "STRING"]
dataItems = []

def compile_error(lineno, line, reason):
	""" 
	compiler_error: Displays a compiler error and stops compilation

	lineno - The line number the error occured on
	line   - The string of the line
	reason - The reason for the error
	"""
	print '[-] SYNTAX ERROR: line ' + str(lineno)
	print '\n\t' + str(line) + '\n'
	print '[-] ' + reason
	print '[-] Compilation terminated'
	exit(1)

def halt_not_found():
	"""
	halt_not_found: Displays a compiler error and stops compilation
	when a HLT instruction is not found
	"""
	print '[-] ERROR: Halt not found'
	print '\n[-] This program does not contain the \'HLT\' instruction and will not terminate.'
	print '[-] Please add a \'HLT\' instruction to the program.'
	print '\n[-] Compilation terminated'
	exit(1)

def verify_line(line, lineno):
	"""
	verify_line: Verify the syntax and argument usage of a line is valid

	line   - The line to verify
	lineno - The number of the line being verified
	"""
	items = line.split()
	info = instructionInfo[items[0]]

	if info is not None:
		argc = info["argc"]

		# Verify number of arguments supplied to instruction
		if len(items)-1 != argc:
			reason = items[0] + ' takes ' + str(argc) + ' argument(s). ' + str(len(items)-1) + ' supplied.'
			compile_error(lineno, line, reason)

		# If registers are allowed. Verify they are used properly
		if info["regs"] == True:
			for item in items:
				if items.index(item) == 0:
					continue

				# Check arg 2 of the mixed instructions
				if items[0] in mixedInstructions and items.index(item) == 2:
					# make sure its an integer
					try:
						argument = int(item)
						continue
					except ValueError:
						reason = 'Argument 2 of ' + items[0] + ' must be an interger.'
						compile_error(lineno, line, reason)
				elif items[0] in tripleInstructions and items.index(item) == 3:
					# make sure its an integer
					try:
						argument = int(item)
						continue
					except ValueError:
						reason = 'Argument 3 of ' + items[0] + ' must be an interger.'
						compile_error(lineno, line, reason)
				elif items[0] == "LDA" and items.index(item) == 2:
					continue

				if not item in registers:
					reason = 'Argument ' + str(items.index(item)) + ' of ' + items[0] + ' is not a valid register.'
					compile_error(lineno, line, reason)
		elif info["argc"] > 0:
			# Verify other arguments are integers
			for item in items:
				if items.index(item) == 0:
					continue

				try:
					argument = int(item)
				except ValueError:
					reason = 'Argument ' + str(items.index(item)) + ' of ' + items[0] + ' is not a valid integer.'
					compile_error(lineno, line, reason)

def replace_lables(code):
	"""
	replace_lables: Replace the labels in the code with their address
	equivalents

	code - The source code as a string
	"""
	words = code.split()
	indexes = [] # indices of replaced labels
	for i in range(0, len(words)):
		if i < len(words):
			word = words[i]
		else:
			break

		if ':' in word:
			# make sure label is unique
			for j,thing in enumerate(words):
				if thing == word and j != i:
					print '[-] ERROR: Duplicate label'
					print '[-] The label \'' + word + '\' has already been defined.'
					print '\n[-] Compilation terminated'
					exit(1)

			# trim ':'
			words.remove(word)
			word = word[:-1]

			# update indexes
			for j,idx in enumerate(indexes):
				if i < j:
					indexes[j] = idx - 1
					words[idx-1] = str(idx - 1)

			# replace all occurences
			for item in words:
				if item == word:
					words[words.index(word)] = str(i)
					indexes.append(i)

	for i,word in enumerate(words):
		# put the newlines back in
		if word in instructions:
			words[i] = '\n' + word


	return ' '.join(words)

def try_data_section(code):
	lines = code.split('\n')

	close_block = False
	size = 0
	if '====' in lines[0]:
		for i,line in enumerate(lines):
			if i == 0:
				continue

			if '====' in line:
				dataSectionPresent = True
				close_block = True
				size = i
				break

			lineCpy = line.split()

			if not lineCpy:
				continue

			dataType = lineCpy[0]
			if not dataType.upper() in dataTypes:
				reason = dataType + ' is not a valid type'
				compile_error(lines.index(line)+1, line, reason)

			try:
				dataName = lineCpy[1]
			except e:
				reason = 'Invald data declaration syntax.\nSyntax: <type> <name> <data>'
				compile_error(lines.index(line)+1, line, reason)

			offset = 2 + len(dataType) + len(dataName)
			data = line[offset:]

			if data is None:
				reason = 'No data provided for data entry'
				compile_error(lines.index(line)+1, line, reason)

			if dataType.upper() == "INT":
				data = data.split()
				data = ''.join(data)

			if dataName.upper() in dataItems:
				reason = 'This data item has already been declared'
				compile_error(lines.index(line)+1, line, reason)

			lines[i] = str(dataTypes.index(dataType.upper())) + ' ' + dataName.upper() + ' ' + data
			dataItems.append(dataName.upper())

		if not close_block:
			compile_error(0, lines[0], 'No closing block found for data section.')

		return True,size,'\n'.join(lines)
	else:
		return False,0,code

def strip_comments(code):
	"""
	strip_comments: Remove the comments from the code

	code - The source code as a string
	"""
	lines = code.split('\n')
	
	for i,line in enumerate(lines):
		lines[i] = line.split(';')[0].upper() # strip the comments and convert to uppercase

	return '\n'.join(lines)

def compile():
	"""
	compile: The main function of the program
	"""
	if len(sys.argv) < 2:
		print 'Usage: python ' + sys.argv[0] + ' <filename> (outputName)'
		print '\n<filename>: The source file to compile'
		print '(outputName): (optional) The name of the file to output. \'.chip\' will automatically be added'
		exit(0)

	codeFile = open(sys.argv[1], 'r')

	lines = codeFile.read()
	
	present,size,lines = try_data_section(lines)

	sections = lines
	if present == True:
		sections = lines.split('\n')
		lines = ''
		for i in range(size+1, len(sections)):
			lines += sections[i] + '\n'

	lines = strip_comments(lines)
	lines = replace_lables(lines)
	lines = lines.split('\n')

	compiledCode = []
	haltFound = False
	for line in lines:
		items = line.split()
		
		if not items: # skip blank lines
			continue

		if items[0] in instructions:
			compiledCode.append(instructions.index(items[0]))

			verify_line(line, lines.index(line)+1)

			for item in items:
				if items.index(item) == 0:
					continue					

				if item in registers:
					argument = registers.index(item)
				elif items[0] == "LDA" and item in dataItems:
					argument = dataItems.index(item)
				else:
					try:
						argument = int(item)
					except ValueError:
						compile_error(lines.index(line)+1, line, 'Invalid argument to instruction')

				compiledCode.append(argument)

			if items[0] == "HLT":
				haltFound = True

		else:
			compile_error(lines.index(line)+1, line, 'Unknown instruction')

	if not haltFound:
		halt_not_found()

	codeFile.close()

	if len(sys.argv) > 2:
		outputName = sys.argv[2] + '.chip'
	else:
		outputName = 'program.chip'

	outputFile = open(outputName, 'w')

	if present:
		for i in range(0, size+1):
			outputFile.write("%s\n" % sections[i])

	for item in compiledCode:
		outputFile.write("%s " % item)

	outputFile.close()

if __name__ == '__main__':
	compile()

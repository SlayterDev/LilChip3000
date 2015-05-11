import os
import sys

instructions = ["PSH", "ADD", "POP", "SET", "MOV", "LOG", "JMP", "HLT"]
registers = ["A", "B", "C", "D", "E", "F", "IP", "SP"]

def compile():
	if len(sys.argv) < 2:
		print 'Usage: python ' + sys.argv[0] + ' <filename> (outputName)'
		exit(0)

	codeFile = open(sys.argv[1], 'r')

	lines = codeFile.readlines()
	compiledCode = []
	for line in lines:
		items = line.split()
		
		if items[0] in instructions:
			compiledCode.append(instructions.index(items[0]))

			for item in items:
				if items.index(item) == 0:
					continue					

				if item in registers:
						argument = registers.index(item)
				else:
					try:
						argument = int(item)
					except ValueError:
						print '[-] ERROR: Syntax line ' + str(lines.index(line)+1)
						print str(item) + ' not a valid instruction argument'
						print '\nCompilation terminated.'
						exit(1)

				compiledCode.append(argument)
		else:
			print '[-] ERROR: Syntax: line ' + str(lines.index(line)+1)
			print str(items[0]) + ' not a valid instruction'
			print '\nCompilation terminated.'
			exit(1)

	codeFile.close()

	if sys.argv[2] is not None:
		outputName = sys.argv[2] + '.chip'
	else:
		outputName = 'program.chip'

	outputFile = open(outputName, 'w')
	for item in compiledCode:
		outputFile.write("%s " % item)

	outputFile.close()

if __name__ == '__main__':
	compile()

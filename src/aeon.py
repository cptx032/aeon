#@()!

# . > run the current function
# {> > maior > executa ate o fim
# {< > menor > executa ate o fim
# {= > igual > executa ate o fim
# {| > diferente > executa ate o fim

import sys

FUNCTION_LIST = []
VAR_LIST = [0]
FUNCTION_COUNTER = 0
VAR_COUNTER = 0

def _print():
	print VAR_LIST[VAR_COUNTER]

def _println():
	print VAR_LIST[VAR_COUNTER],

def get_input():
	VAR_LIST[VAR_COUNTER] = unicode(raw_input())

FUNCTION_LIST.extend([_print,_println,get_input,])

def run(_content):
	global VAR_COUNTER,FUNCTION_COUNTER
	for line in range(len(_content)):
		for char in range(len(_content[line])):
			_c = _content[line][char]
			if _c == "%":
				VAR_LIST[VAR_COUNTER] = _content[line][char+1:-1]
				break
			elif _c == "#":
				break
			elif _c == ".":
				FUNCTION_LIST[FUNCTION_COUNTER]()
			elif _c == ">":
				VAR_COUNTER += 1
				if VAR_COUNTER+1 > len(VAR_LIST):
					VAR_LIST.append(0)
			elif _c == "<":
				VAR_COUNTER -= 1
			elif _c == "v":
				FUNCTION_COUNTER += 1
			elif _c == "^":
				FUNCTION_COUNTER -= 1
			elif _c == "+":
				VAR_LIST[VAR_COUNTER] = str(int(VAR_LIST[VAR_COUNTER])+1)
			elif _c == "-":
				VAR_LIST[VAR_COUNTER] = str(int(VAR_LIST[VAR_COUNTER])-1)
			elif _c == "(":
				if VAR_LIST[VAR_COUNTER] == "0":
					break
				while_block = _content[line][char+1:]
				while VAR_LIST[VAR_COUNTER] != "0":
					run(while_block)
				break

			elif _c == "{": # IF STATEMENT
				if _content[line][char+1] == "=":
					if VAR_LIST[VAR_COUNTER] == VAR_LIST[VAR_COUNTER-1]:
						char += 1
						continue
					else:
						break
				elif _content[line][char+1] == "]":
					if int(VAR_LIST[VAR_COUNTER]) > int(VAR_LIST[VAR_COUNTER-1]):
						char += 1 # fixme
						continue
					else:
						break
				elif _content[line][char+1] == "[":
					if int(VAR_LIST[VAR_COUNTER]) < int(VAR_LIST[VAR_COUNTER-1]):
						char += 1 # fixme
						continue
					else:
						break
				elif _content[line][char+1] == "|":
					if VAR_LIST[VAR_COUNTER] != VAR_LIST[VAR_COUNTER-1]:
						char += 1 # fixme
						continue
					else:
						break
				else:
					print "SYNTAX ERROR"
					sys.exit(1)

def show_intro():
	print "Aeon 1.0.0 (09.04.2014 15:06:19)"
	print "Made using Python 2.6.6 (r266:84292, Nov 27 2010, 19:47:39)"
	print 'Type "help" for show the stack. "quit" to quit'

def show_stack():
	print "-------------VAR-------------"
	for i in range(len(VAR_LIST)):
		print i, "-"if i == VAR_COUNTER else ":", VAR_LIST[i]
	print "----------FUNCTION----------"
	for i in range(len(FUNCTION_LIST)):
		print i, "-"if i == FUNCTION_COUNTER else ":", FUNCTION_LIST[i].__name__

if __name__ == "__main__":
	if len(sys.argv) == 1:
		show_intro()
		while True:
			command = unicode(raw_input("> "))
			if command == "help":
				show_stack()
				continue
			if command == "quit":
				sys.exit(0)
			run(command)
	else:
		_F = open(sys.argv[1])
		_content = _F.readlines()
		_F.close()
		run(_content)

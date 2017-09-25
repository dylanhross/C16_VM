main : main.o cpu.o memory.o instruction.o execution.o debug.o 
	gcc main.o cpu.o memory.o instruction.o execution.o debug.o -Wall -Wextra -o main

main.o : main.c
	gcc main.c -Wall -Wextra -c -o main.o

cpu.o : cpu.c cpu.h error.h
	gcc cpu.c -Wall -Wextra -c -o cpu.o
	
memory.o : memory.c memory.h error.h
	gcc memory.c -Wall -Wextra -c -o memory.o

execution.o : execution.c execution.h error.h
	gcc execution.c -Wall -Wextra -c -o execution.o

debug.o : debug.c debug.h error.h
	gcc debug.c -Wall -Wextra -c -o debug.o
	
instruction.o : instruction.c instruction.h error.h
	gcc instruction.c -Wall -Wextra -c -o instruction.o
	
test : test.c
	gcc test.c cpu.o memory.o -Wall -Wextra -g -o test

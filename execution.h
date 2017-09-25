/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    execution.h
*/


#ifndef EXECUTION_H
#define EXECUTION_H


#include <stdio.h>

#include "cpu.h"
#include "memory.h"
#include "instruction.h"
#include "error.h"
#include "debug.h"


// size of memory block (starting at the program counter) to 
// print in the case of an execution error 
// 16 bytes (2 instructions)
#define PC_MEMBLK_PRNTSZ 0x10


// main method for CPU instruction execution
//  1. read the program counter
//  2. fetch an instruction from that address
//  3. increment the program counter by instruction size
//  4. execute the instruction
//  5. check the CPU status code
//      - stop execution if nonzero status and report status code
//
// loop until a halt instruction is reached
void exec_main(cpucore_t*, sysmem_t*);

    
#endif

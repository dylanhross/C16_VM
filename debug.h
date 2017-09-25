/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    debug.h
*/


#ifndef DEBUG_H
#define DEBUG_H


#include <stdio.h>


#include "cpu.h"
#include "memory.h"
#include "instruction.h"
#include "error.h"


// prints the contents of all CPU registers
void dbg_prntcpuregs(cpucore_t*);


// prints out the value at a system memory location
void dbg_prntmemloc(sysmem_t*, memaddr_t);


// prints out the values stored in a specified block of system memory
// sysmem, start address, size of block
void dbg_prntmemblk(sysmem_t*, memaddr_t, memaddr_t);


// print the entire contents of the stack
void dbg_prntstack(cpucore_t*, sysmem_t*);


// print the bytes of an instruction
void dbg_prntinstr(instr_t*);


#endif

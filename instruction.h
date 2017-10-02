/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    instruction.h
*/


#ifndef INSTRUCTION_H
#define INSTRUCTION_H


#include "cpu.h"
#include "memory.h"
#include "error.h"


// define some byte offsets for the different components of an instruction's data


// instruction size
#define INSTR_SIZE  6

// struct representing an instruction
// format:
//      data (unsigned char 6)
//      a large enough character array to hold an opcode followed by combinations
//      of unsigned char register arguments, float and unsigned short immediate values
//
// the different components of the instruction can be obtained by casting 
// pointers to various positions in the array to pointers to other types:
//
//  [uc][uc][uc][uc][uc][uc]
//  [op][r0]
//  [op][s0----]
//  [op][r0][s1----]
//  [op][r0][r1]
//  [op][r0][r1][r2][r3]
//  [op][r0][r1][s2----]
//  [op][r0][f-------------]
//
// keeping the instruction as an array of unsigned characters will make it
// very straight forward to load instructions from the memory since an instruction
// is equivalent to a 6 byte block of system memory
typedef struct { unsigned char data[INSTR_SIZE]; } instr_t;


// shorthand for various argument access from instruction datastructure
#define INSTR_R0 *(instr->data + 1)
#define INSTR_R1 *(instr->data + 2)
#define INSTR_R2 *(instr->data + 3)
#define INSTR_R3 *(instr->data + 4)
#define INSTR_S0 *((unsigned short*) (instr->data + 1))
#define INSTR_S1 *((unsigned short*) (instr->data + 2))
#define INSTR_S2 *((unsigned short*) (instr->data + 3))
#define INSTR_F  *((float*) (instr->data + 2))


// enum for decoding the various opcodes into CPU instructions
typedef enum {
    // NO OPERATION
    NOOP,   // 0x00
    // stack instructions
    IPSH,   // 0x01
    IPOP,   // 0x02
    FPSH,   // 0x03
    FPOP,   // 0x04
    // memory access instructions
    IRDM,   // 0x05
    IWTM,   // 0x06
    FRDM,   // 0x07
    FWTM,   // 0x08
    // jump instructions
    JUMP,   // 0x09
    IJEQ,   // 0x0A
    IJGT,   // 0x0B
    IJGE,   // 0x0C
    IJLT,   // 0x0D
    IJLE,   // 0x0E
    // binary arithmetic instructions
    AND,    // 0x0F
    OR,     // 0x10
    XOR,    // 0x11
    NOT,    // 0x12
    RSFT,   // 0x13
    LSFT,   // 0x14
    // integer arithmetic instructions
    IADD,   // 0x15
    ISUB,   // 0x16
    IMUL,   // 0x17
    IDIV,   // 0x18
    IMOD,   // 0x19
    // floating point arithmetic instructions
    FADD,   // 0x1A
    FSUB,   // 0x1B
    FMUL,   // 0x1C
    FDIV,   // 0x1D
    // call/return instructions
    CALL,   // 0x1E
    RET,    // 0x1F
    // immediate value instructions
    ISET,   // 0x20
    FSET,   // 0x21
    // move instructions
    IMOV,   // 0x22
    FMOV,   // 0x23
    IMEQ,   // 0x24
    IMNE,   // 0x25
    IMLT,   // 0x26
    IMLE,   // 0x27
    IMGT,   // 0x28
    IMGE,   // 0x29
    // end of execution
    HALT    // 0x2A
} instr_opdecode_t;

// fills an instruction's data array with 0s
void instr_zero(instr_t*);


// fetches an instruction from a specified memory address
void instr_fetch(instr_t*, sysmem_t*, memaddr_t);


// decodes then executes an instruction
void instr_exec(cpucore_t*, sysmem_t*, instr_t*);


#endif

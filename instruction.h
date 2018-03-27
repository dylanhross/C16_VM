/*
    C16_VM_v3
    Dylan H. Ross
    2018/03/25
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    instruction.h
*/


#ifndef INSTRUCTION_H
#define INSTRUCTION_H


#include <stdlib.h>
#include <stdint.h>


#include "memory.h"


// opcodes
typedef enum {
    NONE,
    NOOP,
    HALT, RETN, CALL,
    LODI, LODF,
    INCI, DECI, SETF, LEAI,
    PSHI, POPI, PSHF, POPF, SETI, CMPI,
    STOI, STOF,
    MOVI, MOVF,
    MEQI, MNEI, ADDI, SUBI,
    MGTI, MGEI, MLTI, MLEI, ADDF, SUBF, MULF, DIVF
} opcode_t;


// structure for instruction decoding (binary tree)
typedef struct instr_node {
    // opcode
    opcode_t opcode;
    // pointers to the left and right nodes
    struct instr_node *left;
    struct instr_node *right;
} instr_node_t;


// initialize/delete the instruction tree
instr_node_t* instr_build_tree();
void instr_delete_tree(instr_node_t*);


// decode an instruction at a given memory address, returning the opcode
opcode_t instr_decode(instr_node_t*, sysmem_t*, uint16_t);


#endif

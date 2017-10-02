/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    cpu.h
*/


#ifndef CPU_H
#define CPU_H


#include "memory.h"
#include "error.h"


// initial address for the program counter
#define RPC_INIT 0x0000
// initial stack pointer position
#define RSP_INIT 0xFFFF
// end of execution special status code
#define END_OF_EXECUTION 0xFF


// CPU 16 bit integer register
//      can hold types:
//          [un]signed short
//          [un]signed char
typedef union {
    short            s;
    unsigned short  us;
    char             c;
    unsigned char   uc;
} ireg_t;


// data structure for a single CPU core, contains a complete complement of
// registers of various types. This way multiple cores could theoretically
// be used in the future, although I doubt I will make it that far.
//      registers:
//          rpc -- program counter
//          rsp -- stack pointer                     
//          rbp -- base pointer                      (callee-saved)
//          ia0, ia1, ia2, ia3 -- integer arguments  (callee-saved)
//          irv -- integer return value             (*caller-saved*)
//          fa0, fa1, fa2, fa3 -- float arguments    (callee-saved)
//          frv -- float return value               (*caller-saved*)
typedef struct {
    // core identifier
    unsigned char   cid;
    // status code
    unsigned char   stc;
    // these registers stored simply as unsigned shorts since they
    // are all pointers
    unsigned short  rpc;
    unsigned short  rsp;
    unsigned short  rbp;
    // integer argument registers
    ireg_t          ia0;
    ireg_t          ia1;
    ireg_t          ia2;
    ireg_t          ia3;
    // integer return value
    ireg_t          irv;
    // floating point
    float           fa0;
    float           fa1;
    float           fa2;
    float           fa3;
    // float return value
    float           frv;
} cpucore_t;


// enum mapping the integer cpu core registers to ints
typedef enum {
    rpc,
    rsp,
    rbp,
    ia0,
    ia1,
    ia2,
    ia3,
    irv
} cpucore_iregs_t;


// enum mapping the float cpu core registers to ints
typedef enum {
    fa0,
    fa1,
    fa2,
    fa3,
    frv
} cpucore_fregs_t;


// initialize the cpu, zeros all registers and sets the program counter, stack pointer, 
// and base pointer to their initial values
void cpucore_init(cpucore_t*, unsigned char);


// obtain the value from an integer register
// can be used by functions that need to access the register values but not change them
unsigned short cpucore_getiregv(cpucore_t*, cpucore_iregs_t);


// obtain the value from a floating point register
// can be used by functions that need to access the register values but not change them
float cpucore_getfregv(cpucore_t*, cpucore_fregs_t);


/* STACK INSTRUCTIONS */

// push an integer value onto the stack from integer register, decrement stack
// pointer by the size of the integer value 
void cpucore_ipush(cpucore_t*, sysmem_t*, cpucore_iregs_t);

// pop an integer value from the stack into the integer register destination, 
// increment stack pointer
void cpucore_ipop(cpucore_t*, sysmem_t*, cpucore_iregs_t);

// push a float value onto the stack from float register, decrement stack
// pointer by the size of the float value 
void cpucore_fpush(cpucore_t*, sysmem_t*, cpucore_fregs_t);

// pop a float value from the stack into the float register destination, 
// increment stack pointer
void cpucore_fpop(cpucore_t*, sysmem_t*, cpucore_fregs_t);


/* MEMORY ACCESS INSTRUCTIONS */

// reads an integer value from a memory location into an integer register
void cpucore_irdmem(cpucore_t*, sysmem_t*, cpucore_iregs_t, memaddr_t);

// writes an integer value to a memory location from an integer register
void cpucore_iwtmem(cpucore_t*, sysmem_t*, cpucore_iregs_t, memaddr_t);

// reads a float value from a memory location into a floatregister
void cpucore_frdmem(cpucore_t*, sysmem_t*, cpucore_fregs_t, memaddr_t);

// writes a float value to a memory location from a float register
void cpucore_fwtmem(cpucore_t*, sysmem_t*, cpucore_fregs_t, memaddr_t);


/* JUMP INSTRUCTIONS */

// unconditional jump
void cpucore_jmp(cpucore_t*, memaddr_t);

// jump if two integer registers are equal
void cpucore_ijeq(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, memaddr_t);

// jump if integer register 1 > integer register 2
void cpucore_ijgt(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, memaddr_t);

// jump if integer register 1 >= integer register 2
void cpucore_ijge(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, memaddr_t);

// jump if integer register 1 < integer register 2
void cpucore_ijlt(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, memaddr_t);

// jump if integer register 1 <= integer register 2
void cpucore_ijle(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, memaddr_t);


/* BINARY ARITHMETIC INSTRUCTIONS */

// perform bitwise and operation
// computes i1 & i2 and stores the result in i1
void cpucore_and(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// perform bitwise or operation
// computes i1 | i2 and stores the result in i1
void cpucore_or(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// perform bitwise xor operation
// computes i1 ^ i2 and stores the result in i1
void cpucore_xor(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// perform bitwise not operation
// computes !i and stores the result in i
void cpucore_not(cpucore_t*, cpucore_iregs_t);

// perform a bitwise right shift by specified offset
void cpucore_rshift(cpucore_t*, cpucore_iregs_t, unsigned char);

// perform a bitwise left shift by specified offset
void cpucore_lshift(cpucore_t*, cpucore_iregs_t, unsigned char);


/* INTEGER ARITHMETIC INSTRUCTIONS */

// add an integer register into another
// add i1 + i2 and store the result in i1
void cpucore_iadd(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// subtract an integer register from another 
// add i1 - i2 and store the result in i1
void cpucore_isub(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// multiply an integer register into another
// multiply i1 * i2 and store the result in i1
void cpucore_imul(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// divide an integer register into another 
// divide i1 / i2 and store the result in i1
void cpucore_idiv(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// calculate the modulus of one register divided into another 
// mod i1 % i2 and store the result in i1
void cpucore_imod(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);


/* FLOATING POINT ARITHMETIC INSTRUCTIONS */

// add a floating point register into another
// add i1 + i2 and store the result in i1
void cpucore_fadd(cpucore_t*, cpucore_fregs_t, cpucore_fregs_t);

// subtract a floating point register from another 
// add i1 - i2 and store the result in i1
void cpucore_fsub(cpucore_t*, cpucore_fregs_t, cpucore_fregs_t);

// multiply a floating point register into another
// multiply i1 * i2 and store the result in i1
void cpucore_fmul(cpucore_t*, cpucore_fregs_t, cpucore_fregs_t);

// divide a floating point register into another 
// divide i1 / i2 and store the result in i1
void cpucore_fdiv(cpucore_t*, cpucore_fregs_t, cpucore_fregs_t);


/* CALL/RETURN INSTRUCTIONS */

// call
// push callee-saved registers onto stack
// push current execution address onto stack
// jump to new execution address
void cpucore_call(cpucore_t*, sysmem_t*, memaddr_t);

// ret
// pop execution address off of stack
// jump to execution address
// pop callee-saved registers off of stack
void cpucore_ret(cpucore_t*, sysmem_t*);


/* IMMEDIATE VALUE INSTRUCTIONS */

// set integer register to immediate value
void cpucore_isetr(cpucore_t*, cpucore_iregs_t, unsigned short);

// set floating point register to immediate value
void cpucore_fsetr(cpucore_t*, cpucore_fregs_t, float);


/* MOVE INSTRUCTIONS */

// move integer value from r1 into r2 
void cpucore_imov(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t);

// move floating point value from r1 into r2 
void cpucore_fmov(cpucore_t*, cpucore_fregs_t, cpucore_fregs_t);

// integer move r1 into r2 if r1 == r2
void cpucore_imveq(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);

// integer move r1 into r2 if r1 != r2
void cpucore_imvne(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);

// integer move r1 into r2 if r1 < r2
void cpucore_imvlt(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);

// integer move r1 into r2 if r1 <= r2
void cpucore_imvle(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);

// integer move r1 into r2 if r1 > r2
void cpucore_imvgt(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);

// integer move r1 into r2 if r1 >= r2
void cpucore_imvge(cpucore_t*, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t, cpucore_iregs_t);


#endif

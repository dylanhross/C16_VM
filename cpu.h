/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    cpu.h
*/


#ifndef CPU_H
#define CPU_H


#include <stdint.h>
#include <stdlib.h>
#include "memory.h"
#include "error.h"


// enum for selecting integer registers
// (includes: rsp, rbp, ir0-ir3, irv)
typedef enum {
    RPC,
    RSP,
    RBP,
    IR0, IR1, IR2, IR3,
    IRV
} ireg_t;


// enum for selecting floating point registers
// (includes: fr0-fr3, frv)
typedef enum {
    FR0, FR1, FR2, FR3,
    FRV
} freg_t;

// enum for representing the result of comparisons of two numbers
// NA means uninitialized and will cause instructions that rely on comparisons
// being done ahead of time to fail.
typedef enum {
    NA,
    EQ,
    GT,
    LT
} cmpres_t;


// CPU core data structure
//      registers:
//          rpc -- program counter
//          rsp -- stack pointer                     
//          rbp -- base pointer                      (callee-saved)
//          ir0, ir1, ir2, ir3 -- integer arguments  (callee-saved)
//          irv -- integer return value             (*caller-saved*)
//          fr0, fr1, fr2, fr3 -- float arguments    (callee-saved)
//          frv -- float return value               (*caller-saved*)
typedef struct core {
    
    // miscellaneous CPU core data
    uint8_t     cid;    // core ID (for multiple cores in one VM)
    sysmem_t    *smem;  // pointer to system memory data structure
    cmpres_t    rcmp;   // register for comparisons
    errcode_t   stc;    // status code
    
    // integer registers (stored as unsigned 16-bit)
    uint16_t    rpc;    // program counter
    uint16_t    rsp;    // stack pointer
    uint16_t    rbp;    // base pointer
    uint16_t    ir0;    // general purpose integer register 0
    uint16_t    ir1;    // general purpose integer register 1 
    uint16_t    ir2;    // general purpose integer register 2 
    uint16_t    ir3;    // general purpose integer register 3
    uint16_t    irv;    // integer return value
    
    // floating point registers (stored as 32-bit floats)
    float       fr0;    // general purpose floating point register 0
    float       fr1;    // general purpose floating point register 1
    float       fr2;    // general purpose floating point register 2
    float       fr3;    // general purpose floating point register 3
    float       frv;    // floating point return value
    
    // function pointers
    // no operation
    void (*noop) (struct core*);
    // halt execution
    void (*halt) (struct core*);
    // compare two integer registers, store the result in the rcmp register
    void (*cmpi) (struct core*, ireg_t, ireg_t);
    // set an integer register to an immediate value
    void (*seti) (struct core*, ireg_t, uint16_t);
    // move a value from one integer register to another
    void (*movi) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is EQ
    void (*meqi) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is GT or LT
    void (*mnei) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is GT or EQ
    void (*mgei) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is GT
    void (*mgti) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is LT or EQ
    void (*mlei) (struct core*, ireg_t, ireg_t);
    // move a value from register a to register b if rcmp is LT
    void (*mlti) (struct core*, ireg_t, ireg_t);
    // push and pop int values to and from the stack
    void (*pshi) (struct core*, ireg_t);
    void (*popi) (struct core*, ireg_t);
    // load/store int value from/to memory
    void (*lodi) (struct core*, uint16_t, ireg_t);
    void (*stoi) (struct core*, ireg_t, uint16_t);
    // compute address using a base address, offset and multiplier (1 or 2)
    void (*leai) (struct core*, ireg_t, ireg_t, uint8_t, ireg_t);
    // push and pop float values to and from the stack
    void (*pshf) (struct core*, freg_t);
    void (*popf) (struct core*, freg_t);
    // execute a subroutine starting at a memory address
    void (*call) (struct core*, uint16_t);
    void (*retn) (struct core*);
    // set float register value
    void (*setf) (struct core*, freg_t, float);
    // move float value from one register to another
    void (*movf) (struct core*, freg_t, freg_t);
    // load/store float value from/to memory
    void (*lodf) (struct core*, uint16_t, freg_t);
    void (*stof) (struct core*, freg_t, uint16_t);
    // increment/decrement an integer register
    void (*inci) (struct core*, ireg_t);
    void (*deci) (struct core*, ireg_t);
    // add/subtract values from integer registers A, B result in B
    void (*addi) (struct core*, ireg_t, ireg_t);
    void (*subi) (struct core*, ireg_t, ireg_t);
    // add/subtract/multiply/divide float registers A, B result in B
    void (*addf) (struct core*, freg_t, freg_t);
    void (*subf) (struct core*, freg_t, freg_t);
    void (*mulf) (struct core*, freg_t, freg_t);
    void (*divf) (struct core*, freg_t, freg_t);
    
} core_t;


// Allocates memory for a new CPU core structure and returns a pointer to it.
core_t* core_init(uint8_t, sysmem_t*);


// Frees memory associated with CPU core structure to de-initialize.
void core_delete(core_t*);


#endif

/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    error.h
*/


#ifndef ERROR_H
#define ERROR_H


// define error codes 
typedef enum {
    NO_ERR,             // no error
    ERR_HALT,           // set by halt instruction
    ERR_REGUNREC,       // register unrecognized
    ERR_REGNOTALWD,     // register not allowed
    ERR_RCMPNOTINIT,    // conditional operation with rcmp not initialized
    ERR_STACKOVERFLOW,  // stack overflow
    ERR_STACKUNDERFLOW, // stack underflow
    ERR_MEMACCRWBLK,    // memory access out of read/write block
    ERR_LEAIMULTNOT124, // multiplier for leai not 1, 2, or 4
    ERR_EXECOUTOFROBLK, // execute code from outside of RO memory block
    ERR_DECRZERO,       // decrement 0
    ERR_IREGOVERFLOW,   // integer register overflow
    ERR_IREGUNDERFLOW   // integer register underflow
} errcode_t;


#endif

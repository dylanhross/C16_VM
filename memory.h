/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    memory.h
*/


#ifndef MEMORY_H
#define MEMORY_H


#include "error.h"


/*
Memory Map:
(mostly arbitrarily assigned)
                 heap start                                         heap end
                   0x4000                                           0xEF7E
                     +-------------------------------------------------+
program              |                                                 |    stack           stack
counter init         |                                                 |     min            start
0x0000               |                                                 |    0xF000          0xFFFF
  |                  |                                                 |      +----------------+ 
  |                  |                                                 |      |                |
  V                  V                                                 V      V                V
  [--------------------------------------------------------------------------------------------]
0x0000                                                                  ^    ^              0xFFFF
 min                                                                    |    |               max
address                                                                 |    |             address
                                                                        +----+
                                                                    0xEF7F  0xEFFF
                                                                       terminal 
                                                                      line buffer
*/


// maximum memory address 
#define SYSMEM_MAX_ADDR     0xFFFF


// upper memory address bound for the stack 
// 4096 bytes maximum stack size
#define SYSMEM_STACK_MIN    0xF000


// memory address bounds for terminal output buffer
// lies just above the stack
// 128 ASCII characters
#define SYSMEM_TERMLB_START 0xEF7F
#define SYSMEM_TERMLB_END   0xEFFF


// memory address bounds for heap memory
// 44926 bytes maximum heap size
#define SYSMEM_HEAP_START   0x4000
#define SYSMEM_HEAP_END     0xEF7E


// appropriate C type for representing addresses
typedef unsigned short memaddr_t;


// system memory, each memory location holds a single byte
typedef struct { unsigned char mem[SYSMEM_MAX_ADDR + 1]; } sysmem_t;


// write an integer value to a memory location
unsigned char sysmem_iwrite(sysmem_t*, memaddr_t, unsigned short);


// read an integer value from a memory location
unsigned char sysmem_iread(sysmem_t*, unsigned short*, memaddr_t);


// write an integer value to a memory location
unsigned char sysmem_fwrite(sysmem_t*, memaddr_t, float);


// read a float value from a memory location
unsigned char sysmem_fread(sysmem_t*, float*, memaddr_t);


#endif

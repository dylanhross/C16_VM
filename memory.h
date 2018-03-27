/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    memory.h
*/


#ifndef MEMORY_H
#define MEMORY_H


#include <stdlib.h>

/* memory map:
    0xFFFF -- maximum system memory address
        |
        |   <-- stack space (4 kB)
        |
    0xF05F -- initial stack pointer address, stack grows toward larger addresses
        |
        |   <-- general purpose read/write memory block
        |
    0x1F40
        |
        |   <-- read only memory block (8 kB) for program execution
        |
    0x0000 -- minimum system memory address
    
*/
#define MEMORY_MAXADDR  0xFFFF
#define MEMORY_RWBLKMAX 0xF05F 
#define MEMORY_RWBLKMIN 0x1F40


// Main system memory data structure.
typedef struct sysmem {
    
    // 65536 bytes map to "physical" address space of 0x0000 to 0xFFFF, 
    // inclusive. Addressing is done using uint16_t values.
    uint8_t mem[65536];
    
    // function pointers
    // Set the address in memory to a value of a specified type.
    void (*set_uint8) (struct sysmem*, uint16_t, uint8_t);
    void (*set_uint16) (struct sysmem*, uint16_t, uint16_t);
    void (*set_float) (struct sysmem*, uint16_t, float);
    
    // Get the value of a specified type at an address in memory.
    uint8_t (*get_uint8) (struct sysmem*, uint16_t);
    uint16_t (*get_uint16) (struct sysmem*, uint16_t);
    float (*get_float) (struct sysmem*, uint16_t);

} sysmem_t;


// Allocates space for a new sysmem structure and returns a pointer to it.
sysmem_t* sysmem_init();


// Frees memory associated with sysmem structure to de-initialize.
void sysmem_delete(sysmem_t*);


#endif

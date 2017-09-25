/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    memory.c
*/


#include "memory.h"


// write an integer value to a memory location
unsigned char sysmem_iwrite(sysmem_t *sysmem, memaddr_t addr, unsigned short value) {
    // check the address
    if (addr > SYSMEM_MAX_ADDR) {
        return ERR_SMEM_MAXADR;
    }
    // other memory address bounds checking can be done here as well
    // assign the value to the memory location
    *((unsigned short*)(sysmem->mem + addr)) = value;
    // return 0 for success
    return 0;
}


// read an integer value from a memory location
unsigned char sysmem_iread(sysmem_t *sysmem, unsigned short *dest, memaddr_t addr) {
    // check the address
    if (addr > SYSMEM_MAX_ADDR) {
        return ERR_SMEM_MAXADR;
    }
    // other memory address bounds checking can be done here as well
    // assign the value from the memory location to the dereferenced pointer
    // passed in
    *dest = *((unsigned short*)(sysmem->mem + addr));
    // return 0 for success
    return 0;
}


// write an integer value to a memory location
unsigned char sysmem_fwrite(sysmem_t *sysmem, memaddr_t addr, float value) {
    // check the address
    if (addr > SYSMEM_MAX_ADDR) {
        return ERR_SMEM_MAXADR;
    }
    // other memory address bounds checking can be done here as well
    // assign the value to the memory location
    *((float*)(sysmem->mem + addr)) = value;
    // return 0 for success
    return 0;
}


// read an integer value from a memory location
unsigned char sysmem_fread(sysmem_t *sysmem, float *dest, memaddr_t addr) {
    // check the address
    if (addr > SYSMEM_MAX_ADDR) {
        return ERR_SMEM_MAXADR;
    }
    // other memory address bounds checking can be done here as well
    // assign the value from the memory location to the dereferenced pointer
    // passed in
    *dest = *((float*)(sysmem->mem + addr));
    // return 0 for success
    return 0;
}




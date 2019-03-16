/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    memory.c
*/


#include "memory.h"


// Set the address in memory to a uint8_t value.
void _set_uint8(sysmem_t *smem, uint16_t addr, uint8_t val) {
    *(smem->mem + addr) = val;
}


// Set the address in memory to a uint8_t value.
void _set_uint16(sysmem_t *smem, uint16_t addr, uint16_t val) {
    *((uint16_t*) (smem->mem + addr)) = val;
}


// Set the address in memory to a float value.
void _set_float(sysmem_t *smem, uint16_t addr, float val) {
    *((float*) (smem->mem + addr)) = val;
}


// Get a uint8_t value from an address in memory.
uint8_t _get_uint8(sysmem_t *smem, uint16_t addr) {
    return *((uint8_t*) (smem->mem + addr));
}


// Get a uint16_t value from an address in memory.
uint16_t _get_uint16(sysmem_t *smem, uint16_t addr) {
    return *((uint16_t*) (smem->mem + addr));
}


// Get a float value from an address in memory.
float _get_float(sysmem_t *smem, uint16_t addr) {
    return *((float*) (smem->mem + addr));
}


// Allocates space for a new sysmem structure and returns a pointer to it.
sysmem_t* sysmem_init(uint8_t n_cores) {
    // allocate memory
    sysmem_t *smem = calloc(1, sizeof(sysmem_t));
    // set all of the function pointers
    smem->set_uint8 = &_set_uint8;
    smem->set_uint16 = &_set_uint16;
    smem->set_float = &_set_float;
    smem->get_uint8 = &_get_uint8;
    smem->get_uint16 = &_get_uint16;
    smem->get_float = &_get_float;
    // set the number of cpu cores
    smem->n_cores = n_cores;
    return smem;
}


// Frees memory associated with sysmem structure to de-initialize.
void sysmem_delete(sysmem_t *smem) {
    free(smem);
}

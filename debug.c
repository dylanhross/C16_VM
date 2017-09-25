/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    debug.c
*/


#include "debug.h"


// prints the contents of all CPU registers
void dbg_prntcpuregs(cpucore_t *cpu) {
    printf("\n--> CPU %i REGISTER STATES <--\n", cpu->cid);
    // hexadecimal
    printf("status code:            0x%02X\n", cpu->stc);
    printf("program counter:        0x%04X\n", cpu->rpc);
    printf("stack pointer:          0x%04X\n", cpu->rsp);
    printf("base pointer:           0x%04X\n", cpu->rbp);
    // hexadecimal (unsigned short)
    printf("argument 0 (integer):   0x%04X     (%i)\n", cpu->ia0.us, cpu->ia0.us);
    printf("argument 1 (integer):   0x%04X     (%i)\n", cpu->ia1.us, cpu->ia1.us);
    printf("argument 2 (integer):   0x%04X     (%i)\n", cpu->ia2.us, cpu->ia2.us);
    printf("argument 3 (integer):   0x%04X     (%i)\n", cpu->ia3.us, cpu->ia3.us);
    printf("return value (integer): 0x%04X     (%i)\n", cpu->irv.us, cpu->irv.us);
    // hexadecimal (float)
    printf("argument 0 (float):     0x%08X (%f)\n", *((int*)&cpu->fa0), cpu->fa0);
    printf("argument 1 (float):     0x%08X (%f)\n", *((int*)&cpu->fa1), cpu->fa1);
    printf("argument 2 (float):     0x%08X (%f)\n", *((int*)&cpu->fa2), cpu->fa2);
    printf("argument 3 (float):     0x%08X (%f)\n", *((int*)&cpu->fa3), cpu->fa3);
    printf("return value (float):   0x%08X (%f)\n", *((int*)&cpu->frv), cpu->frv);
    printf("-----------------------------\n\n");
}


// prints out the value at a system memory location
void dbg_prntmemloc(sysmem_t *sysmem, memaddr_t addr) {
    printf("value at 0x%04X: 0x%02X\n", addr, sysmem->mem[addr]);
}


// prints out the values stored in a specified block of system memory
void dbg_prntmemblk(sysmem_t *sysmem, memaddr_t start, memaddr_t size) {
    printf("\n--> CURRENT SYSMEM CONTENTS <--\n");
    if (start + size > SYSMEM_MAX_ADDR) {
        // print a warning that the block overruns the system memory
        printf("! WARNING: dbg_prntmemblk: requested memory block overruns system memory !\n");
    }
    for (memaddr_t addr = start; addr < start + size; addr++) {
        dbg_prntmemloc(sysmem, addr);
    }
    printf("-------------------------------\n\n");
}


// print the entire contents of the stack
void dbg_prntstack(cpucore_t *cpu, sysmem_t *sysmem) {
    printf("\n--> CPU %i STACK CONTENTS <--\n", cpu->cid);
    if (cpu->rsp == RSP_INIT) {
        printf("<empty>\n");
    } else {
        for (memaddr_t i = cpu->rsp; i < RSP_INIT; i++) {
            dbg_prntmemloc(sysmem, i);
        }
    }
    printf("----------------------------\n\n");
}   


// print the bytes of an instruction
void dbg_prntinstr(instr_t *instr) {
    printf("[%02X", *instr->data);
    for (int i = 1; i < INSTR_SIZE; i++) {
        printf("|%02X", *(instr->data + i));
    }

    printf("]\n");
}


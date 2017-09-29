/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    execution.c
*/


#include "execution.h"


// main method for CPU instruction execution
//  1. read the program counter
//  2. fetch an instruction from that address
//  3. increment the program counter by instruction size
//  4. execute the instruction
//  5. check the CPU status code
//      - stop execution if nonzero status and report status code
//
// loop until a halt instruction is reached
void exec_main(cpucore_t *cpu, sysmem_t *sysmem) {
    // count CPU cycles
    unsigned int cycount = 0;
    // initialize an instruction
    instr_t instr;
    // loop while cpu status code is 0 (i.e. no errors)
    while (!cpu->stc) {
        
        // increase the cycle counter
        cycount++;
    
        instr_zero(&instr);
        instr_fetch(&instr, sysmem, cpu->rpc);
        
        // DEBUG
        printf("executing instruction at 0x%04X: ", cpu->rpc);
        dbg_prntinstr(&instr);
        
        
        
        cpu->rpc += INSTR_SIZE;
        instr_exec(cpu, sysmem, &instr);
    }
    // the loop is exited once the halt instruction is received or when an error cpu
    // status code is set
    if (cpu->stc != END_OF_EXECUTION) {
        // a cpu error code has been set, report the code then print the cpu registers,
        // stack contents, the last instruction that was attempted, and a 16 byte memory 
        // block starting at the program counter
        printf("EXECUTION ERROR: cpu %i status code: 0x%02X\n\n", cpu->cid, cpu->stc);
        printf("cycle count: %u\n", cycount);
        printf("last instruction executed:\n");
        dbg_prntinstr(&instr);
        dbg_prntcpuregs(cpu);
        dbg_prntstack(cpu, sysmem);
        dbg_prntmemblk(sysmem, (memaddr_t) cpu->rpc, PC_MEMBLK_PRNTSZ);
    }
}


// loads an assembled binary into sysmem at a specified location from a file
void exec_loadbin(sysmem_t *sysmem, memaddr_t addr, char *fname) {
    // read instructions into sysmem from a file
    FILE *fp = fopen(fname, "rb");
    // find out how many instructions are in the file
    unsigned short n_instr;
    fread(&n_instr, 2, 1, fp);
    // read the instructions directly into sysmem at specified address
    fread(sysmem->mem + addr, INSTR_SIZE, n_instr, fp);
    fclose(fp);
}

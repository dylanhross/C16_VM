

#include <stdio.h>


#include "cpu.h"
#include "memory.h"
#include "instruction.h"
#include "debug.h"
#include "execution.h"





int main() {
    
    // initialize cpu core and system memory
    cpucore_t cpu;
    cpucore_init(&cpu, 0);
    sysmem_t sysmem;
    
    
    // read instructions into sysmem from a file
    exec_loadbin(&sysmem, 0x0000, "test.C16.b");
    

    // print the memory block to see if the instructions got loaded correctly
    dbg_prntmemblk(&sysmem, 0x0000, 0x004C);

    
    exec_main(&cpu, &sysmem);
    //dbg_prntcpuregs(&cpu);
    

    return 0;
}


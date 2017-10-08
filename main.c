

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
    exec_loadbin(&sysmem, 0x0000, "f_isq.C16.b");
    

    // print the memory block to see if the instructions got loaded correctly
    //dbg_prntmemblk(&sysmem, 0x0000, 0x004C);

    
    exec_main(&cpu, &sysmem);
    dbg_prntcpuregs(&cpu);
    dbg_prntstack(&cpu, &sysmem);
    
    /*
    unsigned int a = 8000000, b = 2000000, a_rec;
    unsigned short a_upp, a_low, b_upp, b_low, a_upp_sub, a_low_sub;
    a_low = *((unsigned short*) &a);
    a_upp = a >> 16;
    b_low = *((unsigned short*) &b);
    b_upp = b >> 16;
    printf("a (0x%08X): %u\n", a, a);
    printf("    upper 2 bytes: 0x%04X\n", a_upp);
    printf("    lower 2 bytes: 0x%04X, %u\n", a_low, a_low);
    printf("b (0x%08X): %u\n", b, b);
    printf("    upper 2 bytes: 0x%04X\n", b_upp);
    printf("    lower 2 bytes: 0x%04X, %u\n", b_low, b_low);
    a_upp_sub = a_upp - b_upp;
    a_low_sub = a_low - b_low;
    if (a_low_sub > a_low) {
        //a_low_sub = 0;
        a_upp_sub -= 1;
    }
    printf("after subtraction:\n");
    printf("    upper 2 bytes: 0x%04X\n", a_upp_sub);
    printf("    lower 2 bytes: 0x%04X, %u\n", a_low_sub, a_low_sub);
    a_rec = (a_upp_sub << 16) | (a_low_sub & 0xFFFF);
    printf("\nrecovered (0x%08X): %u\n", a_rec, a_rec);
    printf("subtracted (0x%08X): %u\n", a - b, a - b);
    */
    
    
    float f = 3.9;
    float f2 = f * 0.5F;
    unsigned int ui = *((unsigned int*) &f), subbed;
    unsigned int magic = 0x5f3759df;
    printf(" float (3.9) as unsigned int : 0x%04X\n", ui);
    printf("         right bit shifted 1 : 0x%04X\n", ui >> 1);
    printf("subtracted from magic number : 0x%04X\n", magic - (ui >> 1));
    subbed = magic - (ui >> 1);
    f = *((float *) &subbed);
    printf("               back to float : %f\n", f);
    float f3 = f * (1.5F - (f2 * f * f));
    printf("         after one iteration : %f\n", f3);
    printf("        after two iterations : %f\n", f3 * (1.5F - (f2 * f3 * f3)));
    
    
    unsigned short us1, us2, us3;
    us1 = 0x59DF;
    us2 = 0xCCCD;
    us3 = us1 - us2;
    printf("0x%04X\n", us3);
    

    return 0;
}


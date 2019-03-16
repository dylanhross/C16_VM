/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    test.c
*/


#include "cpu.h"
#include <stdio.h>


void print_memrange(sysmem_t*, uint16_t, uint16_t);
void print_cpuregs(core_t*);
void print_instr_tree(instr_node_t*);


int main() {
    
    /* SETUP */
    sysmem_t *smem = sysmem_init(1);
    core_t *core0 = core_init(0, smem);
    
    instr_node_t *instr_tree =  instr_build_tree();
    
    /*
    core0->seti(core0, IR0, 0x0001);
    core0->seti(core0, IR1, 0x0010);
    core0->seti(core0, IR2, 0x0100);
    core0->seti(core0, IR3, 0x1000);
    core0->setf(core0, FR0, 0.1);
    core0->setf(core0, FR1, 0.01);
    core0->setf(core0, FR2, 0.001);
    core0->setf(core0, FR3, 0.0001);
    
    
    core0->leai(core0, IR0, IR1, 4, IRV);

    printf("--------------------------------------------------------\n");
    print_cpuregs(core0);
    print_memrange(smem, 0xF05F, 0xF0AF);
    printf("\n");

    core0->call(core0, 0x1111);
    core0->seti(core0, IR0, 0x0001);
    core0->seti(core0, IR1, 0x0002);
    core0->seti(core0, IR2, 0x0003);
    core0->seti(core0, IR3, 0x0004);
    core0->addi(core0, IR0, IR1);
    core0->addi(core0, IR2, IR3);
    core0->subi(core0, IR1, IR3);
    core0->movi(core0, IR3, IRV);
    

    
    printf("--------------------------------------------------------\n");
    print_cpuregs(core0);
    print_memrange(smem, 0xF05F, 0xF0AF);
    printf("\n");
    
    core0->retn(core0);
    
    printf("--------------------------------------------------------\n");
    print_cpuregs(core0);
    print_memrange(smem, 0xF05F, 0xF0AF);
    printf("\n");
    */
    
    /* FINISH */
    core_delete(core0);
    sysmem_delete(smem);

    return 0;
}


void print_memrange(sysmem_t *smem, uint16_t start, uint16_t end) {
    uint16_t r = (end - start) / 16;
    r = (end - start) % 16 ? r + 1 : r;
    for (uint16_t i = 0; i < r; i++) {
        printf("[0x%04X] ", i * 16 + start);
        uint16_t j = 0;
        uint16_t jaddr = i * 16 + start;
        while (j < 16 && jaddr < end) {
            printf("%02X ", *(smem->mem + jaddr));
            j++;
            jaddr++;
        }
        printf("\n");
    }
    
}


void print_cpuregs(core_t *core) {
    printf("! STC: %d !\n", core->stc);
    printf("RPC: 0x%04X\n", core->rpc);
    printf("RSP: 0x%04X\n", core->rsp);
    printf("RBP: 0x%04X\n", core->rbp);
    printf("     IR0     IR1     IR2     IR3     IRV\n");
    printf("   %5d   %5d   %5d   %5d   %5d\n", core->ir0, core->ir1, core->ir2, core->ir3, core->irv);
    printf("  0x%04X  0x%04X  0x%04X  0x%04X  0x%04X\n", core->ir0, core->ir1, core->ir2, core->ir3, core->irv);
    printf("     FR0      FR1      FR2      FR3      FRV\n");
    printf("  %7.4f  %7.4f  %7.4f  %7.4f  %7.4f\n", core->fr0, core->fr1, core->fr2, core->fr3, core->frv);
}

void print_instr_tree(instr_node_t *root) {
    
}

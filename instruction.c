/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    instruction.c
*/


#include "instruction.h"


// fills an instruction's data array with 0s
void instr_zero(instr_t *instr) {
    for (unsigned short i = 0; i < INSTR_SIZE; i++) {
        *(instr->data + i) = 0;
    }
}


// fetches an instruction from a specified memory address
void instr_fetch(instr_t *instr, sysmem_t *sysmem, memaddr_t addr) {
    for (unsigned short i = 0; i < INSTR_SIZE; i++) {
        *(instr->data + i) = *(sysmem->mem + addr + i);
    }
}


// decodes then executes an instruction
void instr_exec(cpucore_t *cpu, sysmem_t *sysmem, instr_t *instr) {
    // decode the opcode from the beginning of the instruction
    switch (*instr->data) {
        
        // NO OPERATION
        case NOOP:
            break;
            
        // stack instructions
        case IPSH:
            cpucore_ipush(cpu, sysmem, INSTR_R0);
            break;
        case IPOP:
            cpucore_ipop(cpu, sysmem, INSTR_R0);
            break;
        case FPSH:
            cpucore_fpush(cpu, sysmem, INSTR_R0);
            break;
        case FPOP:
            cpucore_fpop(cpu, sysmem, INSTR_R0);
            break;
            
        // memory access instructions
        case IRDM:
            cpucore_irdmem(cpu, sysmem, INSTR_R0, INSTR_S1);
            break;
        case IWTM:
            cpucore_iwtmem(cpu, sysmem, INSTR_R0, INSTR_S1);
            break;
        case FRDM:
            cpucore_frdmem(cpu, sysmem, INSTR_R0, INSTR_S1);
            break;
        case FWTM:
            cpucore_fwtmem(cpu, sysmem, INSTR_R0, INSTR_S1);
            break;
            
        // jump instructions
        case JUMP:
            cpucore_jmp(cpu, INSTR_S0);
            break;
        case IJEQ:
            cpucore_ijeq(cpu, INSTR_R0, INSTR_R1, INSTR_S2);
            break;
        case IJGT:
            cpucore_ijgt(cpu, INSTR_R0, INSTR_R1, INSTR_S2);
            break;
        case IJGE:
            cpucore_ijge(cpu, INSTR_R0, INSTR_R1, INSTR_S2);
            break;
        case IJLT:
            cpucore_ijlt(cpu, INSTR_R0, INSTR_R1, INSTR_S2);
            break;
        case IJLE:
            cpucore_ijle(cpu, INSTR_R0, INSTR_R1, INSTR_S2);
            break;
            
        // binary arithmetic instructions
        case AND:
            cpucore_and(cpu, INSTR_R0, INSTR_R1);
            break;
        case OR:
            cpucore_or(cpu, INSTR_R0, INSTR_R1);
            break;
        case XOR:
            cpucore_xor(cpu, INSTR_R0, INSTR_R1);
            break;
        case NOT:
            cpucore_not(cpu, INSTR_R0);
            break;
        case RSFT:
            cpucore_rshift(cpu, INSTR_R0, INSTR_S1);
            break;
        case LSFT:
            cpucore_lshift(cpu, INSTR_R0, INSTR_S1);
            break;
            
        // integer arithmetic instructions
        case IADD:
            cpucore_iadd(cpu, INSTR_R0, INSTR_R1);
            break;
        case ISUB:
            cpucore_isub(cpu, INSTR_R0, INSTR_R1);
            break;
        case IMUL:
            cpucore_imul(cpu, INSTR_R0, INSTR_R1);
            break;
        case IDIV:
            cpucore_idiv(cpu, INSTR_R0, INSTR_R1);
            break;
        case IMOD:
            cpucore_imod(cpu, INSTR_R0, INSTR_R1);
            break;
            
        // floating point arithmetic instructions
        case FADD:
            cpucore_fadd(cpu, INSTR_R0, INSTR_R1);
            break;
        case FSUB:
            cpucore_fsub(cpu, INSTR_R0, INSTR_R1);
            break;
        case FMUL:
            cpucore_fmul(cpu, INSTR_R0, INSTR_R1);
            break;
        case FDIV:
            cpucore_fdiv(cpu, INSTR_R0, INSTR_R1);
            break;
            
        // call/return instructions
        case CALL:
            cpucore_call(cpu, sysmem, INSTR_S0);
            break;
        case RET:
            cpucore_ret(cpu, sysmem);
            break;
            
        // immediate value instructions
        case ISET:
            cpucore_isetr(cpu, INSTR_R0, INSTR_S1);
            break;
        case FSET:
            cpucore_fsetr(cpu, INSTR_R0, INSTR_F);
            break;
        
        // end of execution
        case HALT:
            cpu->stc = END_OF_EXECUTION;
            break;
        
        // unrecognized operation
        default:
            // unrecognized opcode, error
            cpu->stc = ERR_UNREC_INSTR;
            break;
    }
}


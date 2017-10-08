/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    cpu.c
*/


#include "cpu.h"

#include "debug.h"
#include <stdio.h>


// initialize the cpu, zeros all registers and sets the program counter, stack pointer, 
// and base pointer to their initial values
void cpucore_init(cpucore_t *cpu, unsigned char id) {
    // set the core id and status code
    cpu->cid = id;
    cpu->stc = 0x00;
    cpu->rpc = RPC_INIT;
    cpu->rsp = RSP_INIT;
    // base pointer starts off pointing at the same position as stack pointer
    cpu->rbp = RSP_INIT;
    // zero out all of the registers
    cpu->ia0.us = 0; cpu->ia1.us = 0;
    cpu->ia2.us = 0; cpu->ia3.us = 0;
    cpu->irv.us = 0;
    // zero out all of the floating point registers
    cpu->fa0 = 0; cpu->fa1 = 0;
    cpu->fa2 = 0; cpu->fa3 = 0;
    cpu->frv = 0;
}


// obtain the value from an integer register
// can be used by functions that need to access the register values but not change them
unsigned short cpucore_getiregv(cpucore_t *cpu, cpucore_iregs_t ireg) {
    switch (ireg) {
        case rpc:
            return cpu->rpc;
            break;
        case rsp:
            return cpu->rsp;
            break;
        case rbp:
            return cpu->rbp;
            break;
        case ia0:
            return cpu->ia0.us;
            break;
        case ia1:
            return cpu->ia1.us;
            break;
        case ia2:
            return cpu->ia2.us;
            break;
        case ia3:
            return cpu->ia3.us;
            break;
        case irv:
            return cpu->irv.us;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_IREG;
            return 0;
    }
}


// obtain the value from a floating point register
// can be used by functions that need to access the register values but not change them
float cpucore_getfregv(cpucore_t *cpu, cpucore_fregs_t freg) {
    switch (freg) {
        case fa0:
            return cpu->fa0;
        case fa1:
            return cpu->fa1;
        case fa2:
            return cpu->fa2;
        case fa3:
            return cpu->fa3;
        case frv:
            return cpu->frv;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
            return 0;
    }
}


/* STACK INSTRUCTIONS */

// push an integer value onto the stack from integer register, decrement stack
// pointer by the size of the integer value 
void cpucore_ipush(cpucore_t *cpu, sysmem_t *sysmem, cpucore_iregs_t ireg) {
    // decrement the stack pointer by 2 bytes
    cpu->rsp -= 0x2;
    if (ireg == rsp) {
        // this is an error, cannot push the stack pointer on the stack
        cpu->stc = ERR_IPUSH_RSP;
        // increment the stack pointer by 2 bytes (restore to previous position)
        cpu->rsp += 0x2;
    } else {
        cpu->stc = sysmem_iwrite(sysmem, cpu->rsp, cpucore_getiregv(cpu, ireg));
    }
}

// pop an integer value from the stack into the integer register destination, 
// increment stack pointer
void cpucore_ipop(cpucore_t *cpu, sysmem_t *sysmem, cpucore_iregs_t ireg) {
    switch (ireg) {
        case rsp:
            // this is an error, cannot push the stack pointer on the stack
            cpu->stc = ERR_IPOP_RSP;
            // decrement the stack pointer by 2 bytes (so that it ends up in 
            // the same place after it gets incremented later
            cpu->rsp -= 0x2;
            break;
        case rpc:
            cpu->stc = sysmem_iread(sysmem, &cpu->rpc, cpu->rsp);
            break;
        case rbp:
            cpu->stc = sysmem_iread(sysmem, &cpu->rbp, cpu->rsp);
            break;
        case ia0:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia0.us, cpu->rsp);
            break;
        case ia1:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia1.us, cpu->rsp);
            break;
        case ia2:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia2.us, cpu->rsp);
            break;
        case ia3:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia3.us, cpu->rsp);
            break;
        case irv:
            cpu->stc = sysmem_iread(sysmem, &cpu->irv.us, cpu->rsp);
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            // decrement the stack pointer by 2 bytes (so that it ends up in 
            // the same place after it gets incremented later
            cpu->rsp -= 0x2;
            break;  
    }
    // increment the stack pointer by 2 bytes
    cpu->rsp += 0x2;
}

// push an integer value onto the stack from integer register, decrement stack
// pointer by the size of the integer value 
void cpucore_fpush(cpucore_t *cpu, sysmem_t *sysmem, cpucore_fregs_t freg) {
    // decrement the stack pointer by 4 bytes
    cpu->rsp -= 0x4;
    cpu->stc = sysmem_fwrite(sysmem, cpu->rsp, cpucore_getfregv(cpu, freg));
}

// pop a float value from the stack into the float register destination, 
// increment stack pointer
void cpucore_fpop(cpucore_t *cpu, sysmem_t *sysmem, cpucore_fregs_t freg) {
    switch (freg) {
        case fa0:
            cpu->stc = sysmem_fread(sysmem, &cpu->fa0, cpu->rsp);
            break;
        case fa1:
            cpu->stc = sysmem_fread(sysmem, &cpu->fa1, cpu->rsp);
            break;
        case fa2:
            cpu->stc = sysmem_fread(sysmem, &cpu->fa2, cpu->rsp);
            break;
        case fa3:
            cpu->stc = sysmem_fread(sysmem, &cpu->fa3, cpu->rsp);
            break;
        case frv:
            cpu->stc = sysmem_fread(sysmem, &cpu->frv, cpu->rsp);
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_FREG;
            // decrement the stack pointer by 4 bytes (so that it ends up in 
            // the same place after it gets incremented later
            cpu->rsp -= 0x4;
            break;  
    }
    // increment the stack pointer by 4 bytes
    cpu->rsp += 0x4;
}


/* MEMORY ACCESS INSTRUCTIONS */

// reads an integer value from a memory location into an integer register
void cpucore_irdmem(cpucore_t *cpu, sysmem_t *sysmem, cpucore_iregs_t ireg, memaddr_t addr) {
    switch (ireg) {
        case rpc:
            cpu->stc = sysmem_iread(sysmem, &cpu->rpc, addr);
            break;
        case rsp:
            cpu->stc = sysmem_iread(sysmem, &cpu->rsp, addr);
            break;
        case rbp:
            cpu->stc = sysmem_iread(sysmem, &cpu->rbp, addr);
            break;
        case ia0:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia0.us, addr);
            break;
        case ia1:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia1.us, addr);
            break;
        case ia2:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia2.us, addr);
            break;
        case ia3:
            cpu->stc = sysmem_iread(sysmem, &cpu->ia3.us, addr);
            break;
        case irv:
            cpu->stc = sysmem_iread(sysmem, &cpu->irv.us, addr);
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;  
    }
}

// writes an integer value to a memory location from an integer register
void cpucore_iwtmem(cpucore_t *cpu, sysmem_t *sysmem, cpucore_iregs_t ireg, memaddr_t addr) {
    sysmem_iwrite(sysmem, addr, cpucore_getiregv(cpu, ireg));
}

// reads a float value from a memory location into a floatregister
void cpucore_frdmem(cpucore_t *cpu, sysmem_t *sysmem, cpucore_fregs_t freg, memaddr_t addr) {
    switch (freg) {
        case fa0:
            cpu->stc = sysmem_fread(sysmem, (float*) &cpu->fa0, addr);
            break;
        case fa1:
            cpu->stc = sysmem_fread(sysmem, (float*) &cpu->fa1, addr);
            break;
        case fa2:
            cpu->stc = sysmem_fread(sysmem, (float*) &cpu->fa2, addr);
            break;
        case fa3:
            cpu->stc = sysmem_fread(sysmem, (float*) &cpu->fa3, addr);
            break;
        case frv:
            cpu->stc = sysmem_fread(sysmem, (float*) &cpu->frv, addr);
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_FREG;
            break;  
    }
}

// writes a float value to a memory location from a float register
void cpucore_fwtmem(cpucore_t *cpu, sysmem_t *sysmem, cpucore_fregs_t freg, memaddr_t addr) {
    sysmem_fwrite(sysmem, addr, cpucore_getfregv(cpu, freg));
}


/* JUMP INSTRUCTIONS */

// unconditional jump
void cpucore_jmp(cpucore_t *cpu, memaddr_t addr) {
    // check the address
    // other memory address bounds checking could be done here as well
    if (addr > SYSMEM_MAX_ADDR) {
        cpu->stc = ERR_SMEM_MAXADR;
    } else {
        cpu->rpc = addr;
    }
}

// jump if two integer registers are equal
void cpucore_ijeq(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, memaddr_t addr) {
    if (ireg1 == ireg2) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg1) == cpucore_getiregv(cpu, ireg2)) {
        // jump if the values at the two registers are equal
        cpucore_jmp(cpu, addr);
    }
}

// jump if integer register 1 > integer register 2
void cpucore_ijgt(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, memaddr_t addr) {
    if (ireg1 == ireg2) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg1) > cpucore_getiregv(cpu, ireg2)) {
        // jump if reg1 > reg2
        cpucore_jmp(cpu, addr);
    }
}

// jump if integer register 1 >= integer register 2
void cpucore_ijge(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, memaddr_t addr) {
    if (ireg1 == ireg2) {    
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg1) >= cpucore_getiregv(cpu, ireg2)) {
        // jump if reg1 >= reg2
        cpucore_jmp(cpu, addr);
    }
}

// jump if integer register 1 < integer register 2
void cpucore_ijlt(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, memaddr_t addr) {
    if (ireg1 == ireg2) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg1) < cpucore_getiregv(cpu, ireg2)) {
        // jump if reg1 < reg2
        cpucore_jmp(cpu, addr);
    }
}

// jump if integer register 1 <= integer register 2
void cpucore_ijle(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, memaddr_t addr) {
    if (ireg1 == ireg2) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg1) <= cpucore_getiregv(cpu, ireg2)) {
        // jump if reg1 <= reg2
        cpucore_jmp(cpu, addr);
    }
}


/* BINARY ARITHMETIC INSTRUCTIONS */

// perform bitwise and operation
// computes i1 & i2 and stores the result in i1
void cpucore_and(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    unsigned short and =  cpucore_getiregv(cpu, ireg1) & cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = and;
            break;
        case ia1:
            cpu->ia1.us = and;
            break;
        case ia2:
            cpu->ia2.us = and;
            break;
        case ia3:
            cpu->ia3.us = and;
            break;
        case irv:
            cpu->irv.us = and;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// perform bitwise or operation
// computes i1 | i2 and stores the result in i1
void cpucore_or(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    unsigned short or =  cpucore_getiregv(cpu, ireg1) | cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = or;
            break;
        case ia1:
            cpu->ia1.us = or;
            break;
        case ia2:
            cpu->ia2.us = or;
            break;
        case ia3:
            cpu->ia3.us = or;
            break;
        case irv:
            cpu->irv.us = or;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// perform bitwise xor operation
// computes i1 ^ i2 and stores the result in i1
void cpucore_xor(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    unsigned short xor =  cpucore_getiregv(cpu, ireg1) ^ cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = xor;
            break;
        case ia1:
            cpu->ia1.us = xor;
            break;
        case ia2:
            cpu->ia2.us = xor;
            break;
        case ia3:
            cpu->ia3.us = xor;
            break;
        case irv:
            cpu->irv.us = xor;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// perform bitwise not operation
// computes ~i and stores the result in i
void cpucore_not(cpucore_t *cpu, cpucore_iregs_t ireg) {
    unsigned short not =  ~cpucore_getiregv(cpu, ireg);
    switch (ireg) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = not;
            break;
        case ia1:
            cpu->ia1.us = not;
            break;
        case ia2:
            cpu->ia2.us = not;
            break;
        case ia3:
            cpu->ia3.us = not;
            break;
        case irv:
            cpu->irv.us = not;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// perform a bitwise right shift by specified offset
void cpucore_rshift(cpucore_t *cpu, cpucore_iregs_t ireg, unsigned char offset) {
    unsigned short shift =  cpucore_getiregv(cpu, ireg) >> offset;
    switch (ireg) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = shift;
            break;
        case ia1:
            cpu->ia1.us = shift;
            break;
        case ia2:
            cpu->ia2.us = shift;
            break;
        case ia3:
            cpu->ia3.us = shift;
            break;
        case irv:
            cpu->irv.us = shift;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// perform a bitwise left shift by specified offset
void cpucore_lshift(cpucore_t *cpu, cpucore_iregs_t ireg, unsigned char offset) {
    unsigned short shift =  cpucore_getiregv(cpu, ireg) << offset;
    switch (ireg) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to and into
            cpu->stc = ERR_BIN_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = shift;
            break;
        case ia1:
            cpu->ia1.us = shift;
            break;
        case ia2:
            cpu->ia2.us = shift;
            break;
        case ia3:
            cpu->ia3.us = shift;
            break;
        case irv:
            cpu->irv.us = shift;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}


/* INTEGER ARITHMETIC INSTRUCTIONS */

/* 
    NOTE: all integer arithmetic is implemented with unsigned short values, 
          I will need to take a look later and see if this still works with
          signed shorts and (un)signed chars and if necessary adjust the 
          implementations accordingly
*/

// add an integer register into another
// add i1 + i2 and store the result in i1
void cpucore_iadd(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    // detect overflow and set the status code
    // cast values to ints to do the addition see if result is greater than
    // unsigned short max (0xFFFF)
    unsigned int sum = (unsigned int) cpucore_getiregv(cpu, ireg1) + 
                       (unsigned int) cpucore_getiregv(cpu, ireg2);
    if (sum > 0xFFFF) {
        cpu->stc = ERR_IADD_OVRFLW;
        // set the value to 0xFFFF
        sum = 0xFFFF;
    }
    switch (ireg1) {
        case rpc:
            // rpc is not allowed to add into
            cpu->stc = ERR_IADD_REGNOTALWD;
            break;
        case rsp:
            cpu->rsp = (unsigned short) sum;
            break;
        case rbp:
            cpu->rbp = (unsigned short) sum;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = (unsigned short) sum;
            break;
        case ia1:
            cpu->ia1.us = (unsigned short) sum;
            break;
        case ia2:
            cpu->ia2.us = (unsigned short) sum;
            break;
        case ia3:
            cpu->ia3.us = (unsigned short) sum;
            break;
        case irv:
            cpu->irv.us = (unsigned short) sum;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// subtract an integer register from another 
// add i1 - i2 and store the result in i1
void cpucore_isub(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    // detect underflow and set the status code
    unsigned short dif = cpucore_getiregv(cpu, ireg1) - cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
            // rpc is not allowed to add into
            cpu->stc = ERR_ISUB_REGNOTALWD;
            break;
        case rsp:
            cpu->rsp = (unsigned short) dif;
            break;
        case rbp:
            cpu->rbp = (unsigned short) dif;
            break;
        // the difference was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = dif;
            break;
        case ia1:
            cpu->ia1.us = dif;
            break;
        case ia2:
            cpu->ia2.us = dif;
            break;
        case ia3:
            cpu->ia3.us = dif;
            break;
        case irv:
            cpu->irv.us = dif;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// multiply an integer register into another
// multiply i1 * i2 and store the result in i1
void cpucore_imul(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    // detect overflow and set the status code
    // cast values to ints to do the addition see if result is greater than
    // unsigned short max (0xFFFF)
    unsigned int prod = (unsigned int) cpucore_getiregv(cpu, ireg1) * 
                       (unsigned int) cpucore_getiregv(cpu, ireg2);
    if (prod > 0xFFFF) {
        cpu->stc = ERR_IMUL_OVRFLW;
        // set the value to 0xFFFF
        prod = 0xFFFF;
    }
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to add into
            cpu->stc = ERR_IADD_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = (unsigned short) prod;
            break;
        case ia1:
            cpu->ia1.us = (unsigned short) prod;
            break;
        case ia2:
            cpu->ia2.us = (unsigned short) prod;
            break;
        case ia3:
            cpu->ia3.us = (unsigned short) prod;
            break;
        case irv:
            cpu->irv.us = (unsigned short) prod;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// divide an integer register into another 
// divide i1 / i2 and store the result in i1
void cpucore_idiv(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    unsigned short quot = cpucore_getiregv(cpu, ireg1) / cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to add into
            cpu->stc = ERR_IDIV_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = quot;
            break;
        case ia1:
            cpu->ia1.us = quot;
            break;
        case ia2:
            cpu->ia2.us = quot;
            break;
        case ia3:
            cpu->ia3.us = quot;
            break;
        case irv:
            cpu->irv.us = quot;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// calculate the modulus of one register divided into another 
// mod i1 % i2 and store the result in i1
void cpucore_imod(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2) {
    unsigned short mod = cpucore_getiregv(cpu, ireg1) % cpucore_getiregv(cpu, ireg2);
    switch (ireg1) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to add into
            cpu->stc = ERR_IMOD_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = mod;
            break;
        case ia1:
            cpu->ia1.us = mod;
            break;
        case ia2:
            cpu->ia2.us = mod;
            break;
        case ia3:
            cpu->ia3.us = mod;
            break;
        case irv:
            cpu->irv.us = mod;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}


/* FLOATING POINT ARITHMETIC INSTRUCTIONS */

// add a floating point register into another
// add i1 + i2 and store the result in i1
void cpucore_fadd(cpucore_t *cpu, cpucore_fregs_t freg1, cpucore_fregs_t freg2) {
    // no overflow checking (yet?) 
    float f2 = cpucore_getfregv(cpu, freg2);
    switch (freg1) {
        case fa0:
            cpu->fa0 += f2;
            break;
        case fa1:
            cpu->fa1 += f2;
            break;
        case fa2:
            cpu->fa2 += f2;
            break;
        case fa3:
            cpu->fa3 += f2;
            break;
        case frv:
            cpu->frv += f2;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}

// subtract a floating point register from another 
// add i1 - i2 and store the result in i1
void cpucore_fsub(cpucore_t *cpu, cpucore_fregs_t freg1, cpucore_fregs_t freg2) {
    float f2 = cpucore_getfregv(cpu, freg2);
    switch (freg1) {
        case fa0:
            cpu->fa0 -= f2;
            break;
        case fa1:
            cpu->fa1 -= f2;
            break;
        case fa2:
            cpu->fa2 -= f2;
            break;
        case fa3:
            cpu->fa3 -= f2;
            break;
        case frv:
            cpu->frv -= f2;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}

// multiply a floating point register into another
// multiply i1 * i2 and store the result in i1
void cpucore_fmul(cpucore_t *cpu, cpucore_fregs_t freg1, cpucore_fregs_t freg2) {
    // no overflow checking (yet?)
    float f2 = cpucore_getfregv(cpu, freg2);
    switch (freg1) {
        case fa0:
            cpu->fa0 *= f2;
            break;
        case fa1:
            cpu->fa1 *= f2;
            break;
        case fa2:
            cpu->fa2 *= f2;
            break;
        case fa3:
            cpu->fa3 *= f2;
            break;
        case frv:
            cpu->frv *= f2;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}

// divide a floating point register into another 
// divide i1 / i2 and store the result in i1
void cpucore_fdiv(cpucore_t *cpu, cpucore_fregs_t freg1, cpucore_fregs_t freg2) {
    float f2 = cpucore_getfregv(cpu, freg2);
    switch (freg1) {
        case fa0:
            cpu->fa0 /= f2;
            break;
        case fa1:
            cpu->fa1 /= f2;
            break;
        case fa2:
            cpu->fa2 /= f2;
            break;
        case fa3:
            cpu->fa3 /= f2;
            break;
        case frv:
            cpu->frv /= f2;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}


/* CALL/RETURN INSTRUCTIONS */

// call
// push callee-saved registers onto stack
// push current execution address onto stack
// jump to new execution address
void cpucore_call(cpucore_t *cpu, sysmem_t *sysmem, memaddr_t addr) {
    // push all callee-saved registers onto stack
    cpucore_ipush(cpu, sysmem, rbp);
    cpucore_ipush(cpu, sysmem, ia0);
    cpucore_ipush(cpu, sysmem, ia1);
    cpucore_ipush(cpu, sysmem, ia2);
    cpucore_ipush(cpu, sysmem, ia3);
    cpucore_fpush(cpu, sysmem, fa0);
    cpucore_fpush(cpu, sysmem, fa1);
    cpucore_fpush(cpu, sysmem, fa2);
    cpucore_fpush(cpu, sysmem, fa3);
    // push current program counter address onto stack
    cpucore_ipush(cpu, sysmem, rpc);
    // jump to the new address
    cpucore_jmp(cpu, addr);
}

// ret
// pop execution address off of stack
// jump to execution address
// pop callee-saved registers off of stack
void cpucore_ret(cpucore_t *cpu, sysmem_t *sysmem) {
    // pop the old execution address off of the stack into the 
    // program counter (effectively jumping back to pre-call position)
    cpucore_ipop(cpu, sysmem, rpc);
    // pop all of the callee-saved registers back into place (in reverse order
    // from when they were pushed on to the stack)
    cpucore_fpop(cpu, sysmem, fa3);
    cpucore_fpop(cpu, sysmem, fa2);
    cpucore_fpop(cpu, sysmem, fa1);
    cpucore_fpop(cpu, sysmem, fa0);
    cpucore_ipop(cpu, sysmem, ia3);
    cpucore_ipop(cpu, sysmem, ia2);
    cpucore_ipop(cpu, sysmem, ia1);
    cpucore_ipop(cpu, sysmem, ia0);
    cpucore_ipop(cpu, sysmem, rbp);
}


/* IMMEDIATE VALUE INSTRUCTIONS */

// set integer register to immediate value
void cpucore_isetr(cpucore_t *cpu, cpucore_iregs_t ireg, unsigned short value) {
    switch (ireg) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to assign immediate value into
            cpu->stc = ERR_IIMM_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = value;
            break;
        case ia1:
            cpu->ia1.us = value;
            break;
        case ia2:
            cpu->ia2.us = value;
            break;
        case ia3:
            cpu->ia3.us = value;
            break;
        case irv:
            cpu->irv.us = value;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// set floating point register to immediate value
void cpucore_fsetr(cpucore_t *cpu, cpucore_fregs_t freg, float value) {
    switch (freg) {
        case fa0:
            cpu->fa0 = value;
            break;
        case fa1:
            cpu->fa1 = value;
            break;
        case fa2:
            cpu->fa2 = value;
            break;
        case fa3:
            cpu->fa3 = value;
            break;
        case frv:
            cpu->frv = value;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}


/* MOVE INSTRUCTIONS */

// move integer value from r1 into r2 
void cpucore_imov(cpucore_t *cpu, cpucore_iregs_t r1, cpucore_iregs_t r2) {
    unsigned short value = cpucore_getiregv(cpu, r1);
    switch (r2) {
        case rpc:
        case rsp:
        case rbp:
            // these registers are not allowed to assign immediate value into
            cpu->stc = ERR_IMOV_REGNOTALWD;
            break;
        // the sum was already computed before, so just cast to short and set the 
        // value of the destination register to that
        case ia0:
            cpu->ia0.us = value;
            break;
        case ia1:
            cpu->ia1.us = value;
            break;
        case ia2:
            cpu->ia2.us = value;
            break;
        case ia3:
            cpu->ia3.us = value;
            break;
        case irv:
            cpu->irv.us = value;
            break;
        default:
            // unrecognized value, error
            cpu->stc = ERR_UNREC_IREG;
            break;
    }
}

// move floating point value from r1 into r2 
void cpucore_fmov(cpucore_t *cpu, cpucore_fregs_t r1, cpucore_fregs_t r2) {
    float value = cpucore_getfregv(cpu, r1);
    switch (r2) {
        case fa0:
            cpu->fa0 = value;
            break;
        case fa1:
            cpu->fa1 = value;
            break;
        case fa2:
            cpu->fa2 = value;
            break;
        case fa3:
            cpu->fa3 = value;
            break;
        case frv:
            cpu->frv = value;
            break;
        default:
            // set the error code
            cpu->stc = ERR_UNREC_FREG;
    }
}

// integer move r1 into r2 if r3 == r4
void cpucore_imveq(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) == cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}

// integer move r1 into r2 if r3 != r4
void cpucore_imvne(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) != cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}

// integer move r1 into r2 if r3 < r4
void cpucore_imvlt(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) < cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}

// integer move r1 into r2 if r3 <= r4
void cpucore_imvle(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) <= cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}

// integer move r1 into r2 if r3 > r4
void cpucore_imvgt(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) > cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}

// integer move r1 into r2 if r3 >= r4
void cpucore_imvge(cpucore_t *cpu, cpucore_iregs_t ireg1, cpucore_iregs_t ireg2, cpucore_iregs_t ireg3, cpucore_iregs_t ireg4) {
    if (ireg3 == ireg4) {
        // cannot compare a register to itself
        cpu->stc = ERR_IREG_CMPTOSELF;
    } else if (cpucore_getiregv(cpu, ireg3) >= cpucore_getiregv(cpu, ireg4)) {
        cpucore_imov(cpu, ireg1, ireg2);
    }
}


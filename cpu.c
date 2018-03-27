/*
    C16_VM_v3
    Dylan H. Ross
    2017/11/24
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    cpu.c
*/


#include "cpu.h"


// Get the value of an integer register.
uint16_t get_ireg_val(core_t *core, ireg_t reg) {
    switch (reg) {
        case RPC:
            return core->rpc;
        case RSP:
            return core->rsp;
        case RBP:
            return core->rbp;
        case IR0:
            return core->ir0;
        case IR1:
            return core->ir1;
        case IR2:
            return core->ir2;
        case IR3:
            return core->ir3;
        case IRV:
            return core->irv;
        default:
            // ERROR -- register unrecognized
            core->stc = ERR_REGUNREC;
            break;
    }
}


/* There are different functions for setting register values based on which 
   registers are allowed to be set.
   _gpr = general purpose integer registers or return value register
   _gp  = general purpose integer registers */ 

// Set the value of an integer register (general purpose integer register).
void set_ireg_val_gp(core_t *core, ireg_t reg, uint16_t val) {
    switch (reg) {
        case RPC:
        case RSP:
        case RBP:
        case IRV:
            // ERROR -- register not allowed
            core->stc = ERR_REGNOTALWD;
            break;
        case IR0:
            core->ir0 = val;
            break;
        case IR1:
            core->ir1 = val;
            break;
        case IR2:
            core->ir2 = val;
            break;
        case IR3:
            core->ir3 = val;
            break;
        default:
            // ERROR -- register unrecognized
            core->stc = ERR_REGUNREC;
            break;
    }
}

// Set the value of an integer register (general purpose integer register
// or return value register).
void set_ireg_val_gpr(core_t *core, ireg_t reg, uint16_t val) {
    switch (reg) {
        case RPC:
            core->rpc = val;
            break;
        case RBP:
            core->rbp = val;
            break;
        case RSP:
            // ERROR -- register not allowed
            core->stc = ERR_REGNOTALWD;
            break;
        case IR0:
            core->ir0 = val;
            break;
        case IR1:
            core->ir1 = val;
            break;
        case IR2:
            core->ir2 = val;
            break;
        case IR3:
            core->ir3 = val;
            break;
        case IRV:
            core->irv = val;
            break;
        default:
            // ERROR -- register unrecognized
            core->stc = ERR_REGUNREC;
            break;
    }
}


// Get the value of a float register.
float get_freg_val(core_t *core, freg_t reg) {
    switch (reg) {
        case FR0:
            return core->fr0;
        case FR1:
            return core->fr1;
        case FR2:
            return core->fr2;
        case FR3:
            return core->fr3;
        case FRV:
            return core->frv;
        default:
            // ERROR -- register unrecognized
            core->stc = ERR_REGUNREC;
            break;
    }
}


void set_freg_val(core_t *core, freg_t reg, float val) {
    switch (reg) {
        case FR0:
            core->fr0 = val;
            break;
        case FR1:
            core->fr1 = val;
            break;
        case FR2:
            core->fr2 = val;
            break;
        case FR3:
            core->fr3 = val;
            break;
        case FRV:
            core->frv = val;
            break;
        default:
            // ERROR -- register unrecognized
            core->stc = ERR_REGUNREC;
            break;
    }
}


// No operation.
void _core_noop(core_t *core) { /* no operation */ }


// Halt execution.
void _core_halt(core_t *core) { 
    // intentionally set an error to stop execution
    core->stc = ERR_HALT;
}


// Compare two integer registers and store the result in the rcmp register.
void _core_cmpi(core_t *core, ireg_t reg_a, ireg_t reg_b) {
    uint16_t val_a = get_ireg_val(core, reg_a);
    uint16_t val_b = get_ireg_val(core, reg_b);
    if (val_a == val_b) {
        core->rcmp = EQ;
    } else if (val_a < val_b) {
        core->rcmp = LT;
    } else {
        core->rcmp = GT;
    }
}


// Set an integer register to an immediate value (general purpose integer
// registers).
void _core_seti(core_t *core, ireg_t reg, uint16_t val) {
    set_ireg_val_gp(core, reg, val);
}


// Move a value from one integer register to another (general purpose integer
// registers or return value register) 
void _core_movi(core_t *core, ireg_t src, ireg_t dst) {
    set_ireg_val_gpr(core, dst, get_ireg_val(core, src));
}


// Move a value from register src to register dst if the value in rcmp is EQ. 
// Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_meqi(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case EQ:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Move a value from register src to register dst if the value in rcmp is 
// GT or LT. Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_mnei(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case LT:
        case GT:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Move a value from register src to register dst if the value in rcmp is 
// GT or EQ. Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_mgei(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case EQ:
        case GT:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Move a value from register src to register dst if the value in rcmp is 
// GT. Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_mgti(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case GT:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Move a value from register src to register dst if the value in rcmp is 
// LT or EQ. Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_mlei(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case EQ:
        case LT:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Move a value from register src to register dst if the value in rcmp is 
// LT. Must follow a cmpi instruction. 
// Regardless of whether the move is performed or not, rcmp is set to NA at the
// end since the comparison may no longer be valid.
void _core_mlti(core_t *core, ireg_t src, ireg_t dst) {
    switch (core->rcmp) {
        case NA:
            // ERROR -- conditional operation with uninitialized rcmp
            core->stc = ERR_RCMPNOTINIT;
            break;
        case LT:
            _core_movi(core, src, dst);
            break;
        default:
            // do nothing
            break;
    }
    // reset rcmp
    core->rcmp = NA;
}


// Push a value onto the stack (general purpose integer registers or return
// value register). Stack grows toward increasing addresses.
void _core_pshi(core_t *core, ireg_t reg) {
    if (core->rsp >= MEMORY_MAXADDR - 2) {
        // ERROR -- stack overflow
        core->stc = ERR_STACKOVERFLOW;
    } else {
        core->smem->set_uint16(core->smem, core->rsp, get_ireg_val(core, reg));
        // increment the stack pointer
        core->rsp += 2;
    }
}


// Pop a value off of the stack (general purpose integer registers or return
// value register).
void _core_popi(core_t *core, ireg_t reg) {
    if (core->rsp <= MEMORY_RWBLKMAX) {
        // ERROR -- stack underflow
        core->stc = ERR_STACKUNDERFLOW;
    } else {
        // decrement the stack pointer
        core->rsp -= 2;
        set_ireg_val_gpr(core, reg, core->smem->get_uint16(core->smem, core->rsp));
    }
}


// Load an integer value from memory into an integer register (general purpose 
// integer registers or return value register).
void _core_lodi(core_t *core, uint16_t addr, ireg_t reg) {
    // memory address bounds checking
    if (addr < MEMORY_RWBLKMIN || addr > MEMORY_RWBLKMAX - 2) {
        // ERROR -- memory access out of read/write block
        core->stc = ERR_MEMACCRWBLK;
    } else {
        set_ireg_val_gpr(core, reg, core->smem->get_uint16(core->smem, addr));
    }
}


// Store an integer value from an integer register in memory (general purpose 
// integer registers or return value register)
void _core_stoi(core_t *core, ireg_t reg, uint16_t addr) {
    // memory address bounds checking
    if (addr < MEMORY_RWBLKMIN || addr > MEMORY_RWBLKMAX - 2) {
        // ERROR -- memory access out of read/write block
        core->stc = ERR_MEMACCRWBLK;
    } else {
        core->smem->set_uint16(core->smem, addr, get_ireg_val(core, reg));
    }
}


// Compute address using a base address, offset and multiplier (1 or 2),
// storing the result in an integer register.
void _core_leai (core_t *core, ireg_t base, ireg_t off, uint8_t mult, ireg_t dest) {
    uint16_t base_v, off_v;
    base_v = get_ireg_val(core, base);
    off_v = get_ireg_val(core, off);
    // multiplier must be 1 or 2 (or 0)
    if (mult > 2) {
        // ERROR -- leai mult > 2
        core->stc = ERR_LEAIMULTGT2;
    } else {
        set_ireg_val_gpr(core, dest, base_v + (off_v << mult));
    }
}


// Push a value onto the stack (general purpose float registers or return
// value register). Stack grows toward increasing addresses.
void _core_pshf(core_t *core, freg_t reg) {
    if (core->rsp >= MEMORY_MAXADDR - 4) {
        // ERROR -- stack overflow
        core->stc = ERR_STACKOVERFLOW;
    } else {
        core->smem->set_float(core->smem, core->rsp, get_freg_val(core, reg));
        // increment the stack pointer
        core->rsp += 4;
    }
}


// Pop a value off of the stack (general purpose float registers or return
// value register).
void _core_popf(core_t *core, freg_t reg) {
    if (core->rsp <= MEMORY_RWBLKMAX) {
        // ERROR -- stack underflow
        core->stc = ERR_STACKUNDERFLOW;
    } else {
        // decrement the stack pointer
        core->rsp -= 4;
        set_freg_val(core, reg, core->smem->get_float(core->smem, core->rsp));
    }
}


// Execute a subroutine starting at a memory address. Preserve general purpose
// register values but not the return value registers.
void _core_call(core_t *core, uint16_t addr) {
    // preserve program counter, base pointer, and all general purpose registers
    // push rpc, rbp, ir0-ir3, fr0-fr3 to stack in that order
    _core_pshi(core, RPC);
    _core_pshi(core, RBP);
    _core_pshi(core, IR0);
    _core_pshi(core, IR1);
    _core_pshi(core, IR2);
    _core_pshi(core, IR3);
    _core_pshf(core, FR0);
    _core_pshf(core, FR1);
    _core_pshf(core, FR2);
    _core_pshf(core, FR3);
    // set the program counter to the subroutine address
    if (addr > MEMORY_RWBLKMIN) {
        // if the address is outside the read-only block that is an error
        core->stc = ERR_EXECOUTOFROBLK;
    } else {
        core->rpc = addr;
    }
}


void _core_retn(core_t *core) {
    // restore registers in reverse order
    _core_popf(core, FR3);
    _core_popf(core, FR2);
    _core_popf(core, FR1);
    _core_popf(core, FR0);
    _core_popi(core, IR3);
    _core_popi(core, IR2);
    _core_popi(core, IR1);
    _core_popi(core, IR0);
    _core_popi(core, RBP);
    _core_popi(core, RPC);

}


// Set a floating point register to an immediate value.
void _core_setf(core_t *core, freg_t reg, float val) {
    set_freg_val(core, reg, val);
}


// Move a floating point value from one register to another.
void _core_movf(core_t *core, freg_t src, freg_t dst) {
    set_freg_val(core, dst, get_freg_val(core, src));
}


// Load a floating point value from a memory address into a float register.
void _core_lodf(core_t *core, uint16_t addr, freg_t reg) {
    set_freg_val(core, reg, core->smem->get_float(core->smem, addr));
}


// Store a floating point value from a float register at an address in memory
void _core_stof(core_t *core, freg_t reg, uint16_t addr) {
    core->smem->set_float(core->smem, addr, get_freg_val(core, reg));
}


// Increment an integer register 
void _core_inci(core_t *core, ireg_t reg) {
    set_ireg_val_gpr(core, reg, get_ireg_val(core, reg) + 1);
}


// Decrement an integer register 
void _core_deci(core_t *core, ireg_t reg) {
    uint16_t val = get_ireg_val(core, reg);
    if (val == 0) {
        // ERROR: decrement 0
        core->stc = ERR_DECRZERO;
    } else { 
        set_ireg_val_gpr(core, reg, val - 1);
    }
}


// add an int register A into another B (store result in B)
void _core_addi(core_t *core, ireg_t rega, ireg_t regb) {
    uint16_t a = get_ireg_val(core, rega);
    uint16_t b = get_ireg_val(core, regb);
    uint16_t max = a > b ? a : b;
    uint16_t sum =  a + b;
    if (sum < max) {
        // ERROR: integer register overflow
        core->stc = ERR_IREGOVERFLOW;
    } else {
        set_ireg_val_gpr(core, regb, sum);
    }
}


// subtract an int register A from another B (store result in B)
void _core_subi(core_t *core, ireg_t rega, ireg_t regb) {
    uint16_t a = get_ireg_val(core, rega);
    uint16_t b = get_ireg_val(core, regb);
    uint16_t diff = b - a;
    if (diff > b) {
        // ERROR: integer register underflow
        core->stc = ERR_IREGUNDERFLOW;
    } else {
        set_ireg_val_gpr(core, regb, diff);
    }
}


// Add float register A into another B, store result in B
void _core_addf(core_t *core, freg_t rega, freg_t regb) {
    float a = get_freg_val(core, rega);
    float b = get_freg_val(core, regb);
    set_freg_val(core, regb, a + b);
}


// subtract float register A from another B, store result in B
void _core_subf(core_t *core, freg_t rega, freg_t regb) {
    float a = get_freg_val(core, rega);
    float b = get_freg_val(core, regb);
    set_freg_val(core, regb, b - a);
}


// Multiply float register A by another B, store result in B
void _core_mulf(core_t *core, freg_t rega, freg_t regb) {
    float a = get_freg_val(core, rega);
    float b = get_freg_val(core, regb);
    set_freg_val(core, regb, a * b);
}


// divide float register B by another A, store result in B
void _core_divf(core_t *core, freg_t rega, freg_t regb) {
    float a = get_freg_val(core, rega);
    float b = get_freg_val(core, regb);
    set_freg_val(core, regb, b / a);
}


// Allocates memory for a new CPU core structure and returns a pointer to it.
core_t* core_init(uint8_t cid, sysmem_t *smem) {
    // allocate (zeroed) memory
    core_t *core = calloc(1, sizeof(core_t));
    // initialize data structure values
    core->cid  = cid;
    core->rsp  = MEMORY_RWBLKMAX; // rsp starts at bottom of stack space
    core->smem = smem;
    core->rcmp = NA;
    core->stc  = NO_ERR;
    // set all of the function pointers
    core->noop = &_core_noop;
    core->halt = &_core_halt;
    core->cmpi = &_core_cmpi;
    core->seti = &_core_seti;
    core->movi = &_core_movi;
    core->meqi = &_core_meqi;
    core->mnei = &_core_mnei;
    core->mgei = &_core_mgei;
    core->mgti = &_core_mgti;
    core->mlei = &_core_mlei;
    core->mlti = &_core_mlti;
    core->pshi = &_core_pshi;
    core->popi = &_core_popi;
    core->lodi = &_core_lodi;
    core->stoi = &_core_stoi;
    core->leai = &_core_leai;
    core->pshf = &_core_pshf;
    core->popf = &_core_popf;
    core->call = &_core_call;
    core->retn = &_core_retn;
    core->setf = &_core_setf;
    core->movf = &_core_movf;
    core->lodf = &_core_lodf;
    core->stof = &_core_stof;
    core->inci = &_core_inci;
    core->deci = &_core_deci;
    core->addi = &_core_addi;
    core->subi = &_core_subi;
    core->addf = &_core_addf;
    core->subf = &_core_subf;
    core->mulf = &_core_mulf;
    core->divf = &_core_divf;
    return core;
}


// Frees memory associated with CPU core structure to de-initialize.
void core_delete(core_t *core) {
    free(core);
}


// Decodes an instruction at a specified memory address and executes it.
void core_decode(core_t *core, sysmem_t *smem, uint16_t addr, uint8_t bit_offset) {
    // decode the byte at the specified memory address bit by bit, starting at
    // the offset 
}


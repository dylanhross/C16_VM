/*
    A simple Virtual Machine --> try #2
    Dylan H. Ross
    
    error.h
*/


#ifndef ERROR_H
#define ERROR_H


// CPU error codes (unsigned char) start at 0x01
#define ERR_IPUSH_RSP       0x01
#define ERR_IPOP_RSP        0x02
#define ERR_UNREC_IREG      0x03
#define ERR_UNREC_FREG      0x04
#define ERR_IREG_CMPTOSELF  0x05
#define ERR_IADD_OVRFLW     0x06
#define ERR_IADD_REGNOTALWD 0x07
#define ERR_ISUB_REGNOTALWD 0x08
#define ERR_ISUB_UNDRFLW    0x09
#define ERR_IMUL_OVRFLW     0x0A
#define ERR_IDIV_REGNOTALWD 0x0B
#define ERR_IMOD_REGNOTALWD 0x0C
#define ERR_BIN_REGNOTALWD  0x0D
#define ERR_IIMM_REGNOTALWD 0x0F
#define ERR_IMOV_REGNOTALWD 0x10


// memory error codes (unsigned char) start at 0x41
#define ERR_SMEM_MAXADR     0x41


// instruction error codes (unsigned char) start at 0x51
#define ERR_UNREC_INSTR     0x51


#endif



#include <stdio.h>
#include <stdlib.h>


#include "cpu.h"




typedef struct {
    unsigned char data[6];
} instr_t2;


#define FFFF *((float*) (ins.data + 2))

int main() {


    instr_t2 ins;

    for (unsigned char i = 0; i < 6; i++) {
        *(ins.data + i) = 0x00;
    }
    
     

    *(ins.data) = 0x0C;
    *(ins.data + 1) = 0x03;
    FFFF = 123.456;
    
    
    
    printf("[%02X", *ins.data);
    for (unsigned char i = 1; i < 6; i++) {
        printf("|%02X", *(ins.data + i));
    }
    printf("]\n");

    
    return 0;
}

    // initialize cpu core and system memory
    cpucore_t cpu;
    cpucore_init(&cpu, 0);
    sysmem_t sysmem;
    
    
    memaddr_t mad = 0x0000;
    instr_t instr;
    cpucore_iregs_t ireg;
    cpucore_fregs_t freg;
    instr_opdecode_t op;
    
    
    // ISET ia1 0x0005
    instr_zero(&instr);
    op = ISET;
    ireg = ia1;
    *(instr.data) = op;
    *(instr.data + 1) = ireg;
    *((unsigned short*) (instr.data + 2)) = 0x0005;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // ISET ia2 0x0001
    instr_zero(&instr);
    op = ISET;
    ireg = ia2;
    *(instr.data) = op;
    *(instr.data + 1) = ireg;
    *((unsigned short*) (instr.data + 2)) = 0x0001;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // FSET fa0 4
    instr_zero(&instr);
    op = FSET;
    freg = fa0;
    *(instr.data) = op;
    *(instr.data + 1) = freg;
    *((float*) (instr.data + 2)) = 4;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // FSET fa1 2
    instr_zero(&instr);
    op = FSET;
    freg = fa1;
    *(instr.data) = op;
    *(instr.data + 1) = freg;
    *((float*) (instr.data + 2)) = 2;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // FDIV fa0 fa1
    instr_zero(&instr);
    op = FDIV;
    freg = fa0;
    *(instr.data) = op;
    *(instr.data + 1) = freg;
    freg = fa1;
    *(instr.data + 2) = freg;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // IADD ia0 ia2
    instr_zero(&instr);
    op = IADD;
    ireg = ia0;
    *(instr.data) = op;
    *(instr.data + 1) = ireg;
    ireg = ia2;
    *(instr.data + 2) = ireg;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
    
    // IJLT ia0 ia1 (4 * INSTR_SIZE)
    instr_zero(&instr);
    op = IJLT;
    ireg = ia0;
    *(instr.data) = op;
    *(instr.data + 1) = ireg;
    ireg = ia1;
    *(instr.data + 2) = ireg;
    *((unsigned short*) (instr.data + 3)) = 4 * INSTR_SIZE;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;
   
    // FADD frv fa0
    instr_zero(&instr);
    op = FDIV;
    freg = frv;
    *(instr.data) = op;
    *(instr.data + 1) = freg;
    freg = fa0;
    *(instr.data + 2) = freg;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }
    mad += INSTR_SIZE;


    // HALT
    instr_zero(&instr);
    op = HALT;
    *(instr.data) = op;
    for (memaddr_t i = 0; i < INSTR_SIZE; i++) {
        *(sysmem.mem + mad + i) = *(instr.data + i);
    }


    

    
    exec_main(&cpu, &sysmem);
    
    
    
    
    
    
    

    return 0;
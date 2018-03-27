/*
    C16_VM_v3
    Dylan H. Ross
    2018/03/25
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    instruction.c
*/


#include "instruction.h"


// initialize a new inst_node
instr_node_t* instr_node_init(opcode_t opcode) {
    instr_node_t *inode = malloc(sizeof(instr_node_t));
    inode->opcode = opcode;
    inode->left = NULL;
    inode->right = NULL;
}


// delete an inst_node
void instr_node_delete(instr_node_t *inode) {
    free(inode);
}


// initialize/delete the instruction tree
instr_node_t* instr_build_tree() {

}


void instr_delete_tree() {

}


// decode an instruction at a given memory address, returning the opcode
opcode_t instr_decode(instr_node_t *instr_tree, sysmem_t *smem, uint16_t addr) {

}



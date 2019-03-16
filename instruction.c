/*
    C16_VM_v3
    Dylan H. Ross
    2018/03/25
    
    The third (and I pray final) iteration of my toy 16-bit virtual machine.
    
    instruction.c
*/


#include "instruction.h"

#include <stdio.h>

// initialize a new inst_node
instr_node_t* instr_node_init(opcode_t opcode) {
    instr_node_t *inode = malloc(sizeof(instr_node_t));
    inode->opcode = opcode;
    inode->left = NULL;
    inode->right = NULL;
    return inode;
}


// delete an inst_node
void instr_node_delete(instr_node_t *inode) {
    free(inode);
}


// find the maximum depth of the tree
uint16_t max_tree_depth(instr_node_t *root) {
    if (!root) {
        return 0;
    }
    uint16_t right = 1 + max_tree_depth(root->left);
    uint16_t left = 1 + max_tree_depth(root->right);
    return left > right ? left : right;
}

// find the minimum depth of the tree
uint16_t min_tree_depth(instr_node_t *root) {
    if (!root) {
        return 0;
    }
    uint16_t right = 1 + min_tree_depth(root->left);
    uint16_t left = 1 + min_tree_depth(root->right);
    return left < right ? left : right;
}

// initialize/delete the instruction tree
instr_node_t* instr_build_tree() {
    instr_node_t *root = NULL;
    instr_node_t *current = root;
    instr_node_t *temp = NULL;

    // loop from the first defined opcode (NOOP) to the last defined opcode (DIVF)
    for (opcode_t op = NOOP; op <= DIVF; op++) {
        // set the current pointer back to the root
        printf("%u\n", op);
        
        if (!root) {
            printf("root is NULL\n");
            root = instr_node_init(op);
        } else {
            // set the current pointer back to the root
            current = root;
            // find min/max tree depth
            uint16_t max_depth = max_tree_depth(root);
            uint16_t min_depth = min_tree_depth(root);
            printf("max depth: %u min depth: %u\n", max_depth, min_depth);
            // if min == max then the tree is full
            if (max_depth == min_depth) {
                // replace the root node with new nodes
                instr_node_t *new = instr_node_init(NONE);
                new->right = instr_node_init(op);
                temp = root;
                root = new;
                new->left = temp;
            }

            /*
            if (current->left || current->right) {

            } else {

            }
            */
        }
    }
    return root;
}


void instr_delete_tree() {

}


// decode an instruction at a given memory address, returning the opcode
opcode_t instr_decode(instr_node_t *instr_tree, sysmem_t *smem, uint16_t addr) {

}



//
// Created by schrodinger on 1/9/20.
//

#include "../ri.h"
#include "../parser.h"
#include "test_utils.h"

int main() {
    test_asm_RI("tlti $ra, 123", 132776059)
    test_asm_RI("tlti $ra, -123", 132841349)
    test_asm_RI("tltiu $t9, 14", 120258574)
    test_asm_RI("bgezall $t9, 1", 120848383)
}
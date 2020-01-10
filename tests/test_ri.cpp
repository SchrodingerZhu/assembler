//
// Created by schrodinger on 1/9/20.
//

#include <ri.h>
#include <test_utils.h>

int main() {
    test_asm(RI, "tlti $ra, 123", 132776059)
    test_asm(RI, "tlti $ra, -123", 132841349)
    test_asm(RI, "tltiu $t9, 14", 120258574)
    test_asm(RI, "bgezall $t9, 0", 120848383)
    test_asm(RI, "bgezall $t9, 0x400004", 120782848)
}
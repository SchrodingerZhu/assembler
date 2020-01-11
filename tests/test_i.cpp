//
// Created by schrodinger on 1/9/20.
//

#include <i.h>
#include <test_utils.h>

int main() {
    test_asm(I, "addi $t1, $t5, -100", 564789148)
    test_asm(I, "addiu $t1, $ra, 50", 669581362)
    test_asm(I, "andi $t8, $v0, 1", 811073537)
    test_asm(I, "bne $v0, $a2, 0x00400000", 0x1446ffff)
    test_asm(I, "lb $k0, 24($k1)", 2205810712)
    test_asm(I, "sh $fp, -55($zero)", 2753494985)
    test_asm(I, "bne $t0, $t1, 0x0040000c", 0x15090002)
    test_asm(I, "sw $ra, 0($sp)", 0xafbf0000)
}
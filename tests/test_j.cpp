//
// Created by schrodinger on 1/9/20.
//

#include <j.h>
#include <test_utils.h>

int main() {
    test_asm(J, "j 0x400000", 0x08100000)
    test_asm(J, "jal 4194304", 0x0c100000)
}
//
// Created by schrodinger on 1/9/20.
//

#include "../r.h"
#include "../parser.h"
#include "test_utils.h"

int main() {
    test_asm_R("or  $t0, $a1, $v1", 0x00a34025)
    test_asm_R("add $t0, $t1, $t2", 0x012a4020)
    test_asm_R("syscall", 0x0000000c)
    test_asm_R("jr $fp", 0x03c00008)
    test_asm_R("srl $v0, $fp, 5", 0x001e1142)
    test_asm_R("sub $s4, $s5, $s6", 45522978)
    test_asm_R("jalr $v0", 4257801)
    test_asm_R("jalr $t0, $v0", 4210697)
}
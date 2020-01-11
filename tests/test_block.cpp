//
// Created by schrodinger on 1/10/20.
//
#include <test_utils.h>

#define TEST_MODE

#include "../include/thead_parser.h"

static size_t BLOCK_TEST_NUM = 0;

void test_block(const std::string &content, std::vector<uint32_t> expected) {
    parser_shared::global_address = BASE_ADDR;
    parser_shared::finished.clear();
    parser_shared::labels.clear();
    parser_shared::label_queue.clear();
    std::istringstream input_stream(content);
    source = &input_stream;
    std::clog << "testing asm block #" << BLOCK_TEST_NUM++ << ": ";
    run_parsing();
    assert_eq(parser_shared::finished.size(), expected.size());
    for (int i = 0; i < expected.size(); i++) {
        std::clog << "\nline #" << i << std::endl;
        display_bits(parser_shared::finished[i].__content, expected[i]);
        assert_eq(parser_shared::finished[i].__content, expected[i]);
    }
    std::clog << "[SUCCESS]\n" << std::endl;
}

int main() {
    test_mode(true);
    test_block("L: add $t0, $t1, $t2\n"
               "M: j L", {0x012a4020, 0x08100000});
    test_block("L: add $t0, $t1, $t2\n"
               "M: bne $v0, $v1, L\n"
               "add $t8, $t9, $ra \n"
               "jal M", {0x012a4020, 0x1443fffe, 0x033fc020, 0x0c100001});
    test_block("addi    $sp,   $sp, -8 \n"
               "sw      $ra, 0($sp) \n"
               "j LABEL_DONE\n"
               "LABEL_DONE: \n"
               "\n"
               "jr $ra", {0x23bdfff8, 0xafbf0000, 0x08100003, 0x03e00008});
    std::vector<uint32_t> list = {
            0x23bdfff8,
            0xafb00004,
            0xafbf0000,
            0x14800002,
            0x20a20001,
            0x08100012,
            0x14a00004,
            0x2084ffff,
            0x20050001,
            0x0c100000,
            0x08100012,
            0x00808020,
            0x20a5ffff,
            0x0c100000,
            0x2204ffff,
            0x00402820,
            0x0c100000,
            0x08100012,
            0x8fb00004,
            0x8fbf0000,
            0x23bd0008,
            0x03e00008,
    };
    auto text = "########################################################\n"
                "# Ackermann's function implementation using MIPS assembly\n"
                "# C interface:\n"
                "#            int AckermannFunc (int m,int n)\n"
                "########################################################\n"
                "\n"
                "\n"
                "\n"
                "# Preconditions:   \n"
                "#   1st parameter ($a0) m\n"
                "#   2nd parameter ($a1) n\n"
                "# Postconditions:\n"
                "#   result in ($v0) = value of A(m,n)\n"
                "\n"
                "# we are going to use $s0 as a temporary registers to store m sometimes\n"
                "\n"
                "AckermannFunc:   \n"
                "             # make space on stack\n"
                "            addi    $sp,   $sp, -8          \n"
                "            # preserve registers used by this function \n"
                "              sw      $s0, 4($sp)\n"
                "            # preserve return address            \n"
                "              sw      $ra, 0($sp)     \n"
                "\n"
                "            # move the parameter registers to temporary  - no, only when nec.\n"
                "\n"
                "LABEL_IF:      # check whether m==0\n"
                "            # if not then branch to LABEL_ELSE_IF\n"
                "            bne   $a0, $zero, LABEL_ELSE_IF\n"
                "\n"
                "            #  code for \"result = n+1\"\n"
                "            addi   $v0,   $a1,   1\n"
                "\n"
                "            # jump to LABEL_DONE\n"
                "            j LABEL_DONE\n"
                "\n"
                "\n"
                "LABEL_ELSE_IF:\n"
                "            # check whether n==0\n"
                "            #if not then branch to LABEL_ELSE\n"
                "            bne $a1, $zero,   LABEL_ELSE\n"
                "\n"
                "            # need to call A(m-1,1)\n"
                "            # so initiate $a0,$a1 with m-1 and 1 \n"
                "            addi   $a0,   $a0,   -1\n"
                "            addi   $a1,   $zero,   1\n"
                "\n"
                "\n"
                "            # call AckermannFunc\n"
                "            jal   AckermannFunc\n"
                "            # Return value already in $v0         \n"
                "\n"
                "            # jump to LABEL_DONE\n"
                "            j LABEL_DONE\n"
                "\n"
                "LABEL_ELSE:      # This block may be a bit tricky !\n"
                "            ###################################\n"
                "\n"
                "            # Save \"m\" to preserve it for the second ackermann call\n"
                "              add     $s0, $a0, $zero\n"
                "\n"
                "            # call to acker(m, (n - 1))\n"
                "              addi    $a1, $a1, -1\n"
                "              jal AckermannFunc\n"
                "            # return value will be used very soon!\n"
                "\n"
                "            # call to acker(m-1, acker(m, (n - 1)))\n"
                "            # Take the \"m\" we saved and decrement it to be the new \"m-1\" :)\n"
                "           addi    $a0, $s0, -1\n"
                "           add     $a1, $v0, $zero\n"
                "           jal AckermannFunc\n"
                "\n"
                "            # jump to LABEL_DONE\n"
                "              j   LABEL_DONE\n"
                "\n"
                "LABEL_DONE:            \n"
                "            # ALREADY loaded the return value register $v0 with result.\n"
                "\n"
                "            # restore registers used by this function\n"
                "            lw   $s0,   4($sp)\n"
                "            # restore return address \n"
                "            lw   $ra,   0($sp)\n"
                "            # restore stack pointer\n"
                "            addi   $sp,   $sp,   8\n"
                "\n"
                "            # return from this function\n"
                "            jr $ra";
    test_block(text, list);
}
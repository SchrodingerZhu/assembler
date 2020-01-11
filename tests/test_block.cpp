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
}
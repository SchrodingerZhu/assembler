//
// Created by schrodinger on 1/10/20.
//
#include <test_utils.h>

#define TEST_MODE

#include "thread_parser.h"

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
    test_block("R:\tadd $s0, $s1, $s2\t#r instructions\n"
               "\taddu $s0, $s1, $s2\n"
               "\tsub $s0, $s1, $s2\n"
               "\tsubu $s0, $s1, $s2\n"
               "\tand $s0, $s1, $s2\n"
               "\tor $s0, $s1, $s2\n"
               "\tnor $s0, $s1, $s2\n"
               "        xor $t0, $t1, $t2\n"
               "\tdiv $t1, $t2\n"
               "\tdivu $t4, $t2\n"
               "\tsll $s0, $s1, 10\n"
               "\tsrl $s0, $s1, 10\n"
               "\tslt $s0, $s1, $s3\n"
               "\tsltu $s0, $s1, $s3\n"
               "\tjr $ra\n"
               "\tmfhi $t3\n"
               "\tmflo $s2\n"
               "\tmult $t2, $t3\n"
               "\tmultu $t1, $t0\n"
               "\t\n"
               "\n"
               "\n"
               "I:\taddi $s0, $s1, -100\t#i instructions\n"
               "\taddiu $s0, $s1, 100\n"
               "\tandi $s0, $s1, 100\n"
               "\tori $s0, $s1, 100\n"
               "\tslti $s0, $s1, 100\n"
               "\tsltiu $s0, $s1, 100\n"
               "\tbeq $s0, $s1, I\n"
               "\tbne $s0, $s1, R\n"
               "\tbgez $s0, I\n"
               "\tbltz $s1, I\n"
               "\tlw $s0, 100($s1)\n"
               "\tsw $s0, 100($s1)\n"
               "\tlui $s0, 100\n"
               "\n"
               "\n"
               "\n"
               "\n"
               "J:\tj I\t\t\t#j instructions\n"
               "\tjal R", {
            0b00000010001100101000000000100000,
            0b00000010001100101000000000100001,
            0b00000010001100101000000000100010,
            0b00000010001100101000000000100011,
            0b00000010001100101000000000100100,
            0b00000010001100101000000000100101,
            0b00000010001100101000000000100111,
            0b00000001001010100100000000100110,
            0b00000001001010100000000000011010,
            0b00000001100010100000000000011011,
            0b00000000000100011000001010000000,
            0b00000000000100011000001010000010,
            0b00000010001100111000000000101010,
            0b00000010001100111000000000101011,
            0b00000011111000000000000000001000,
            0b00000000000000000101100000010000,
            0b00000000000000001001000000010010,
            0b00000001010010110000000000011000,
            0b00000001001010000000000000011001,
            0b00100010001100001111111110011100,
            0b00100110001100000000000001100100,
            0b00110010001100000000000001100100,
            0b00110110001100000000000001100100,
            0b00101010001100000000000001100100,
            0b00101110001100000000000001100100,
            0b00010010000100011111111111111001,
            0b00010110000100011111111111100101,
            0b00000110000000011111111111110111,
            0b00000110001000001111111111110110,
            0b10001110001100000000000001100100,
            0b10101110001100000000000001100100,
            0b00111100000100000000000001100100,
            0b00001000000000000000000000010011,
            0b00001100000000000000000000000000,
    });
    
}

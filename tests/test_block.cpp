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
    parser_shared::global_line_count = 0;
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
    test_block(".text\n"
               "R:\tadd $s0, $s1, $s2\t#r instructions\n"
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
    test_block(".data\n"
               "prompt_m: .asciiz   \"m=\"\n"
               "prompt_n: .asciiz   \"n=\"\n"
               "msg1:   .asciiz   \"Ackermann(\"\n"
               "msg2:   .asciiz   \")=\"\n"
               "comma:  .asciiz \",\"   \n"
               "endl:   .asciiz   \"\\n\"\n"
               ".text\n"
               "P1: \n"
               "    add $t0, $t1, $t2\n"
               "P2: addu $sp, $t1, $v0  # comment #\n"
               "    addi $sp, $sp, -8\n"
               "    addiu $a0, $v0, 1 # $ra, comment, PPP:\n"
               "    and $v0, $v0, $t6\n"
               "    andi $v0, $v0, -1\n"
               "    \n"
               "    j P1\n"
               "    j P2\n"
               "    jal P1\n"
               "    # comment line PP:\n"
               "    jal P2\n"
               "    div $t2, $t1\n"
               "P3: \n"
               "    # also a comment\n"
               "    divu $t2, $t3 # comment\n"
               "    mult $a0, $a1\n"
               "    multu $a1, $a2\n"
               "    nor $v0, $fp,                       $ra\n"
               "    jr $ra\n"
               "    jr $t0\n"
               "    or $t3, $t4, $t5\n"
               "    ori $t1, $ra, 0\n"
               "    sll $t0, $t1, 31\n"
               "    sllv $t1, $t2, $t3\n"
               "    sra $ra, $ra, 1\n"
               "    srav $ra, $sp, $v1\n"
               "    srl  $t0, $t1, 12\n"
               "    srlv $t0, $t1, $t3\n"
               "    \n"
               "            \n"
               "    sub $s0, $s7, $zero\n"
               "    subu $k0, $k1, $gp\n"
               "    xor $v0, $v1, $k0\n"
               "    xori $v1, $zero, 123\n"
               "    lui $v1, 65535\n"
               "    slt $ra,                        $gp, $sp\n"
               "    sltu $t0, $t1, $t2\n"
               "    slti $v0, $v1, 5261\n"
               "    sltiu $v0, $v1, 5524\n"
               "L1: beq  $t0, $t1, L1\n"
               "L2: beq  $t0, $t2, L2\n"
               "__special1:\n"
               "    bgez $v0, P1\n"
               "    bgezal $v1, __special1\n"
               "    bgtz $k0, __special2 \n"
               "__special2:    \n"
               "    blez $k1,       __special1 # comment \n"
               "    bltzal $s4, P3\n"
               "    bltz $ra, __special2\n"
               "    bne $v0, $v1, L2\n"
               "    jalr $v0, $k1\n"
               "    teq $v0, $v1\n"
               "    teqi $ra, 123\n"
               "    tne $k0, $k1\n"
               "    tnei $gp, 0\n"
               "    tge $v0, $v1\n"
               "    tgei $fp, 0\n"
               "    tgeu $fp, $gp\n"
               "    tgeiu $v0, 999\n"
               "    \n"
               "    tlt $v0, $v1\n"
               "    tlti $fp, 0\n"
               "    tltu $fp, $gp\n"
               "    tltiu $v0, 999\n"
               "    lb $ra, -12($sp)\n"
               "    lb $ra, 0($sp)\n"
               "    lb $v0, 12($v1)\n"
               "    lbu $ra, -12($sp)\n"
               "    lbu $ra, 0($sp)\n"
               "    lbu $v0, 12($v1)\n"
               "    lh  $t0, 5000($sp)\n"
               "    lh $ra, 0($sp)\n"
               "    lh $v0, 12($v1)\n"
               "    lhu  $t0, 5000($gp)\n"
               "    lhu $ra, 0($fp)\n"
               "    lhu $v0, 12($v0)\n"
               "    lhu  $t0, -5000($gp)\n"
               "    lw $ra, -1($fp)\n"
               "    lw $v0, 24($v0)\n"
               "    lwl $ra, -1($fp)\n"
               "    lwl $v0, 24($v0)\n"
               "    lwr $ra, -1($fp)\n"
               "    lwr $v0, 24($v0)\n"
               "    ll  $ra, -1($fp)\n"
               "    ll  $v0, 24($v0)\n"
               "    \n"
               "    \n"
               "    \n"
               "    \n"
               "    \n"
               "    \n"
               "    \n"
               "    \n"
               "    sb $ra, -12($sp)\n"
               "    sb $ra, 0($sp)\n"
               "    sb $v0, 12($v1)\n"
               "    sh $ra, -12($sp)\n"
               "    sh $ra, 0($sp)\n"
               "    sh $v0, 12($v1)\n"
               "    sw  $t0, 5000($sp)\n"
               "    sw $ra, 0($sp)\n"
               "    sw $v0, 12($v1)\n"
               "    swl  $t0, 5000($gp)\n"
               "    swl $ra, 0($fp)\n"
               "    swl $v0, 12($v0)\n"
               "    swr  $t0, -5000($gp)\n"
               "    swr $ra, -1($fp)\n"
               "    swr $v0, 24($v0)\n"
               "    sc $ra, -1($fp)\n"
               "    sc $v0, 24($v0)\n"
               "    sc $ra, -1($fp)\n"
               "    \n"
               "    mfhi $ra\n"
               "    mflo $sp\n"
               "    mthi $t0\n"
               "    mtlo $v0\n"
               "    \n"
               "    ",
               {0b00000001001010100100000000100000,
                0b00000001001000101110100000100001,
                0b00100011101111011111111111111000,
                0b00100100010001000000000000000001,
                0b00000000010011100001000000100100,
                0b00110000010000101111111111111111,
                0b00001000000000000000000000000000,
                0b00001000000000000000000000000001,
                0b00001100000000000000000000000000,
                0b00001100000000000000000000000001,
                0b00000001010010010000000000011010,
                0b00000001010010110000000000011011,
                0b00000000100001010000000000011000,
                0b00000000101001100000000000011001,
                0b00000011110111110001000000100111,
                0b00000011111000000000000000001000,
                0b00000001000000000000000000001000,
                0b00000001100011010101100000100101,
                0b00110111111010010000000000000000,
                0b00000000000010010100011111000000,
                0b00000001011010100100100000000100,
                0b00000000000111111111100001000011,
                0b00000000011111011111100000000111,
                0b00000000000010010100001100000010,
                0b00000001011010010100000000000110,
                0b00000010111000001000000000100010,
                0b00000011011111001101000000100011,
                0b00000000011110100001000000100110,
                0b00111000000000110000000001111011,
                0b00111100000000111111111111111111,
                0b00000011100111011111100000101010,
                0b00000001001010100100000000101011,
                0b00101000011000100001010010001101,
                0b00101100011000100001010110010100,
                0b00010001000010011111111111111111,
                0b00010001000010101111111111111111,
                0b00000100010000011111111111011011,
                0b00000100011100011111111111111110,
                0b00011111010000000000000000000000,
                0b00011011011000001111111111111100,
                0b00000110100100001111111111100010,
                0b00000111111000001111111111111101,
                0b00010100010000111111111111111000,
                0b00000000010000001101100000001001,
                0b00000000010000110000000000110100,
                0b00000111111011000000000001111011,
                0b00000011010110110000000000110110,
                0b00000111100011100000000000000000,
                0b00000000010000110000000000110000,
                0b00000111110010000000000000000000,
                0b00000011110111000000000000110001,
                0b00000100010010010000001111100111,
                0b00000000010000110000000000110010,
                0b00000111110010100000000000000000,
                0b00000011110111000000000000110011,
                0b00000100010010110000001111100111,
                0b10000011101111111111111111110100,
                0b10000011101111110000000000000000,
                0b10000000011000100000000000001100,
                0b10010011101111111111111111110100,
                0b10010011101111110000000000000000,
                0b10010000011000100000000000001100,
                0b10000111101010000001001110001000,
                0b10000111101111110000000000000000,
                0b10000100011000100000000000001100,
                0b10010111100010000001001110001000,
                0b10010111110111110000000000000000,
                0b10010100010000100000000000001100,
                0b10010111100010001110110001111000,
                0b10001111110111111111111111111111,
                0b10001100010000100000000000011000,
                0b10001011110111111111111111111111,
                0b10001000010000100000000000011000,
                0b10011011110111111111111111111111,
                0b10011000010000100000000000011000,
                0b11000011110111111111111111111111,
                0b11000000010000100000000000011000,
                0b10100011101111111111111111110100,
                0b10100011101111110000000000000000,
                0b10100000011000100000000000001100,
                0b10100111101111111111111111110100,
                0b10100111101111110000000000000000,
                0b10100100011000100000000000001100,
                0b10101111101010000001001110001000,
                0b10101111101111110000000000000000,
                0b10101100011000100000000000001100,
                0b10101011100010000001001110001000,
                0b10101011110111110000000000000000,
                0b10101000010000100000000000001100,
                0b10111011100010001110110001111000,
                0b10111011110111111111111111111111,
                0b10111000010000100000000000011000,
                0b11100011110111111111111111111111,
                0b11100000010000100000000000011000,
                0b11100011110111111111111111111111,
                0b00000000000000001111100000010000,
                0b00000000000000001110100000010010,
                0b00000001000000000000000000010001,
                0b00000000010000000000000000010011,});
    test_case(data_part, {
        auto data = generate_data();
        for (auto i : data) {
            std::cout << i << std::endl;
        }
    })
}

//
// Created by schrodinger on 1/10/20.
//

#include <i.h>
#include <instructions_types.h>
#include <bitset>
IInstruction::IInstruction(uint8_t opcode, std::initializer_list<I_ORDER> order) : opcode(opcode) {
    std::memcpy(this->order, order.begin(), 3);
}

inline uint8_t parse_brs() {
    eat_sep();
    if (unlikely(at_line_end() || line[counter++] != '(')) {
        throw parse_error(absl::StrCat("wrong format of instruction: ", line));
    }
    uint8_t res = parse_register();
    eat_sep();
    if (unlikely(at_line_end() || line[counter++] != ')')) {
        throw parse_error(absl::StrCat("wrong format of instruction: ", line));
    }
    return res;
}

Instruction generate_I(const char * inst) {
    auto A = IMap.at(inst);
    auto res = Instruction {.INST_I = {.C = 0, .t = 0, .s = 0, .op = A.opcode}};
    for (auto i : A.order) {
        switch (i) {
            case rt1:
                res.INST_I.t = 1;
                break;
            case rt_:
                res.INST_I.t = parse_register();
                break;
            case rs_:
                res.INST_I.s = parse_register();
                break;
            case imm:
                solve_imm(res, inst);
                break;
            case brs:
                res.INST_I.s = parse_brs();
                break;
            case rt0:
            case non:
                break;
            default:
                unreachable();
        }
    }
    if (inst[0] == 'b') res.INST_I.C = ((res.INST_I.C - address) >> 2u) - 1;
    return res;
}

const absl::flat_hash_map<std::string, IInstruction> IMap = {
        {"addi",  {0b001000, {rt_, rs_, imm}}},
        {"addiu", {0b001001, {rt_, rs_, imm}}},
        {"andi",  {0b001100, {rt_, rs_, imm}}},
        {"beq",   {0b000100, {rs_, rt_, imm}}},
        {"bgez",  {0b000001, {rt1, rs_, imm}}},
        {"bgtz",  {0b000111, {rt0, rs_, imm}}},
        {"blez",  {0b000110, {rt0, rs_, imm}}},
        {"bltz",  {0b000001, {rt0, rs_, imm}}},
        {"bne",   {0b000101, {rs_, rt_, imm}}},
        {"lb",    {0b100000, {rt_, imm, brs}}},
        {"lbu",   {0b100100, {rt_, imm, brs}}},
        {"lh",    {0b100001, {rt_, imm, brs}}},
        {"lhu",   {0b100101, {rt_, imm, brs}}},
        {"lui",   {0b001111, {rt_, imm, non}}},
        {"lw",    {0b100011, {rt_, imm, brs}}},
        {"lwc1",  {0b110001, {rt_, imm, brs}}},
        {"ori",   {0b001101, {rt_, rs_, imm}}},
        {"sb",    {0b101000, {rt_, imm, brs}}},
        {"slti",  {0b001010, {rt_, rs_, imm}}},
        {"sltiu", {0b001011, {rt_, rs_, imm}}},
        {"sh",    {0b101001, {rt_, imm, brs}}},
        {"sw",    {0b101011, {rt_, imm, brs}}},
        {"swc1",  {0b111001, {rt_, imm, brs}}},
        {"xori",  {0b001110, {rt_, rs_, imm}}},
};

void solve_imm(Instruction &res, const char *inst) {
    auto s = counter;
    try {
        res.INST_I.C = parse_num<int16_t>();
    }
    catch (const parse_error &e) {
        if (inst[0] == 'b') {
            parser_shared::push_label_queue(s, parser_shared::I);
            res.INST_I.C = 0;
        } else {
            throw parse_error{e};
        }
    }
}

//
// Created by schrodinger on 1/8/20.
//

#include <r.h>

const absl::flat_hash_map<std::string, RInstruction> RMap = {
        {"add",     {0b100000, {rd, rs, rt}}},
        {"addu",    {0b100001, {rd, rs, rt}}},
        {"and",     {0b100100, {rd, rs, rt}}},
        {"break",   {0b001101, {no, no, no}}},
        {"div",     {0b011010, {rs, rt, no}}},
        {"divu",    {0b011011, {rs, rt, no}}},
        {"jalr",    {0b001001, {rd, rs, no}}},
        {"jr",      {0b001000, {rs, no, no}}},
        {"mfhi",    {0b010000, {rd, no, no}}},
        {"mflo",    {0b010010, {rd, no, no}}},
        {"mthi",    {0b010001, {rs, no, no}}},
        {"mtlo",    {0b010011, {rs, no, no}}},
        {"mult",    {0b011000, {rs, rt, no}}},
        {"multu",   {0b011001, {rs, rt, no}}},
        {"nor",     {0b100111, {rd, rs, rt}}},
        {"or",      {0b100101, {rd, rs, rt}}},
        {"sll",     {0b000000, {rd, rt, sa}}},
        {"sllv",    {0b000100, {rd, rt, rs}}},
        {"slt",     {0b101010, {rd, rs, rt}}},
        {"sltu",    {0b101011, {rd, rs, rt}}},
        {"sra",     {0b000011, {rd, rt, sa}}},
        {"srav",    {0b000111, {rd, rt, rs}}},
        {"srl",     {0b000010, {rd, rt, sa}}},
        {"srlv",    {0b000110, {rd, rt, rs}}},
        {"sub",     {0b100010, {rd, rs, rt}}},
        {"subu",    {0b100011, {rd, rs, rt}}},
        {"syscall", {0b001100, {no, no, no}}},
        {"xor",     {0b100110, {rd, rs, rt}}},
};

Instruction generate_R(const char *inst) {
    if (std::strcmp(inst, "jalr") == 0) {
        return parse_JALR();
    }
    auto A = RMap.at(inst);
    auto d = 0;
    auto s = 0;
    auto t = 0;
    auto S = 0;
    auto n = 0;
    while (n < 3 && A.order[n] != no) {
        switch (A.order[n++]) {
            case rd:
                d = parse_register();
                break;
            case rs:
                s = parse_register();
                break;
            case rt:
                t = parse_register();
                break;
            case sa:
                S = parse_num<uint8_t>();
                break;
            default:
                unreachable();
        }
    }
    return Instruction{
            .INST_R = {
                    .f = A.func,
                    .S = static_cast<uint32_t>(S),
                    .d = static_cast<uint32_t>(d),
                    .t = static_cast<uint32_t>(t),
                    .s = static_cast<uint32_t>(s),
                    .op = 0,
            }
    };
}

Instruction parse_R() {
    return generate_R(next_word().data());
}

Instruction parse_JALR() {
    uint8_t s = parse_register(), d = 0;
    if (eat_whitespace(), !at_line_end() && peek() == ',') {
        d = s;
        s = parse_register();
    } else {
        d = 31;
    }
    return Instruction{
            .INST_R =  {
                    .f = 0b001001,
                    .S = 0,
                    .d = d,
                    .t = 0,
                    .s = s,
                    .op = 0
            }
    };
}

RInstruction::RInstruction(uint8_t b, std::initializer_list<Order> order) : func(b) {
    std::memcpy(this->order, order.begin(), 3);
}


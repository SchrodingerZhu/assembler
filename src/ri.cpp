//
// Created by schrodinger on 1/9/20.
//

#include <ri.h>

const absl::flat_hash_map<std::string, uint8_t> RIMap = {
        {"bltz",    0b00000},
        {"bgez",    0b00001},
        {"bltzl",   0b00010},
        {"bgezl",   0b00011},
        {"tgei",    0b01000},
        {"tgeiu",   0b01001},
        {"tlti",    0b01010},
        {"tltiu",   0b01011},
        {"teqi",    0b01100},
        {"tnei",    0b01110},
        {"bltzal",  0b10000},
        {"bgezal",  0b10001},
        {"bltzall", 0b10010},
        {"bgezall", 0b10011},
        {"synci",   0b11111},
};

Instruction generate_RI(const char *inst) {
    auto R = RIMap.at(inst);
    auto s = parse_register();
    int16_t offset;
    auto c = counter;
    try {
        offset = parse_num<int16_t>(true);
        if (inst[0] == 'b') {
            offset = ((offset - address) >> 2u) - 1u;
        }
    } catch (parse_error &e) {
        if (inst[0] == 'b') {
            parser_shared::push_label_queue(c, parser_shared::RI);
            offset = 0;
        } else {
            throw parse_error{e};
        }
    }
    return Instruction{
            .INST_RI = {
                    .C = static_cast<uint32_t>(offset),
                    .R = R,
                    .s = s,
                    .op = 1,
            }
    };
}

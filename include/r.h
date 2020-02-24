//
// Created by schrodinger on 1/8/20.
//

#ifndef ASSEMBLER_R_H
#define ASSEMBLER_R_H

#include "global.h"
#include <absl/container/flat_hash_map.h>
#include "instructions_types.h"
#include "parser.h"

enum Order : uint8_t {
    rd, rs, rt, sa, no
};

struct RInstruction {
    uint8_t func: 6, op: 6;
    Order order[3]{};

    RInstruction(uint8_t op, uint8_t b, std::initializer_list<Order>);
};

extern const absl::flat_hash_map<std::string, RInstruction> RMap;

Instruction generate_R(const char *inst);

Instruction parse_JALR();

Instruction parse_R();

#endif //ASSEMBLER_R_H

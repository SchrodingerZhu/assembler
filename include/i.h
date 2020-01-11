//
// Created by schrodinger on 1/10/20.
//

#ifndef ASSEMBLER_I_H
#define ASSEMBLER_I_H

#include "global.h"
#include "instructions_types.h"
#include <absl/container/flat_hash_map.h>
#include "parser.h"

enum I_ORDER : uint8_t {
    rt0 = 0,
    rt1 = 1,
    rt_,
    rs_,
    imm,
    brs,
    non
};

struct IInstruction {
    uint8_t opcode : 6;
    I_ORDER order[3]{};

    IInstruction(uint8_t opcode, std::initializer_list<I_ORDER>);
};

void solve_imm(Instruction &, const char *inst);

Instruction generate_I(const char *inst);

extern const absl::flat_hash_map<std::string, IInstruction> IMap;

inline Instruction parse_I() {
    return generate_I(next_word().data());
}

#endif //ASSEMBLER_I_H

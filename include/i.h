//
// Created by schrodinger on 1/10/20.
//
/*
 *
 * I_H
 * ---
 * I-Type Definitions
 *
 */
#ifndef ASSEMBLER_I_H
#define ASSEMBLER_I_H

#include "global.h"
#include "instructions_types.h"
#include <absl/container/flat_hash_map.h>
#include "parser.h"

/*
 *
 * enum I_ORDER
 * -------
 * Represents the Order of Arguments when Parsing.
 *
 */
enum I_ORDER : uint8_t {
    rt0 = 0,
    rt1 = 1,
    rt_,
    rs_,
    imm,
    brs,
    non,
    rt8 = 8,
    rt9 = 9,
    rta = 0xa,
    rtb = 0xb,
    rt0xc = 0xc,
    rt0xe = 0xe,
    rt0x10 = 0x10,
    rt0x11 = 0x11,
};

/*
 *
 * struct IInstruction
 * -------------------
 * The Instruction Format of a I-Type Instruction.
 *
 */
struct IInstruction {
    uint8_t opcode : 6;
    I_ORDER order[3]{};

    IInstruction(uint8_t opcode, std::initializer_list<I_ORDER>);
};

/*
 *
 * void solve_imm(Instruction &, const char *inst)
 * -----------------------------------------------
 * Parse the Immediate Data.
 *
 */
void solve_imm(Instruction &, const char *inst);

Instruction generate_I(const char *inst);

extern const absl::flat_hash_map<std::string, IInstruction> IMap; //The I-Type Dictionary

inline Instruction parse_I() {
    return generate_I(next_word().data());
}

#endif //ASSEMBLER_I_H

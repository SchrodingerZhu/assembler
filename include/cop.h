//
// Created by schrodinger on 1/9/20.
//
/*
 *
 * ASSEMBLER_COP_H
 * -------------------------------
 * CO-PROCESSOR RELATED DEFINITIONS
 *
 */
#ifndef ASSEMBLER_COP_H
#define ASSEMBLER_COP_H

#include "global.h"
#include <cctype>
#include <string_view>
#include "instructions_types.h"
#include "parser.h"

#define ERET (Instruction { .__content = 0b01000010000000000000000000011000 })

/*
 *
 * [FAST_PATH] bool is_cop(std::string_view inst)
 * --------------------------------------------
 * Check whether a instruction is a co-processor instruction.
 *
 */
FAST_PATH bool is_cop(std::string_view inst) {
    return inst == "mfc0" || inst == "mtc0" || inst == "erec";
}

/*
 *
 * Instruction generate_COP(const char *inst)
 * -------------------------------------------
 * Generate a COP instruction according to the current line.
 *
 */
Instruction generate_COP(const char *inst);

/*
 *
 * [FAST_PATH] Instruction parse_COP()
 * -------------------------------------------
 * Generate a COP instruction according to the current line.
 *
 */
FAST_PATH Instruction parse_COP() {
    auto word = next_word();
    return generate_COP(word.data());
}

#endif //ASSEMBLER_COP_H

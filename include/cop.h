//
// Created by schrodinger on 1/9/20.
//
#ifndef ASSEMBLER_COP_H
#define ASSEMBLER_COP_H

#include "global.h"
#include <cctype>
#include <string_view>
#include "instructions_types.h"
#include "parser.h"

/// constant ERET instrcuction
#define ERET (Instruction { .__content = 0b01000010000000000000000000011000 })

/*!
 * [FAST_PATH] Check whether a instruction is a co-processor instruction.
 * @returns
 * the checking result
 */
FAST_PATH bool is_cop(/*! instruction name */ std::string_view inst) {
    return inst == "mfc0" || inst == "mtc0" || inst == "erec";
}

/*!
 * Generate a COP instruction according based on the content in the `thread_local` line.
 * @returns
 * the instruction value
 */
Instruction generate_COP(/*! instruction name */ std::string_view inst);

/*!
 * [FAST_PATH] Generate a COP instruction according to the current line.
 * @returns
 * the instruction value
 */
FAST_PATH Instruction parse_COP() {
    auto word = next_word();
    return generate_COP(word.data());
}

#endif //ASSEMBLER_COP_H

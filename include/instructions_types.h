//
// Created by schrodinger on 1/8/20.
//

#ifndef ASSEMBLER_INSTRUCTIONS_TYPES_H
#define ASSEMBLER_INSTRUCTIONS_TYPES_H

#include "global.h"

/*!
 * Instruction is a union of all sorts of MIPS instruction format
 */
union Instruction {
    /// __content stands for the whole instruction in uint32_t format
    uint32_t __content;
    /// INST_R stands for the R-Type instruction.
    struct __R {
        uint32_t
                f: 6,
                S: 5,
                d: 5,
                t:5,
                s: 5,
                op: 6;
    } INST_R;

    /// INST_RI stands for the RI-Type instruction.
    struct __RI {
        uint32_t C: 16, R: 5, s: 5, op: 6;
    } INST_RI;
    /// INST_I stands for the I-Type instruction.
    struct __I {
        uint32_t C: 16, t: 5, s: 5, op: 6;
    } INST_I;
    /// INST_J stands for the J-Type instruction.
    struct __J {
        uint32_t A: 26, op: 6;
    } INST_J;
    /// INST_J stands for the coprocessor instruction.
    struct __COP {
        uint32_t A: 11, d: 5, t: 5, R: 5, op: 6;
    } COP;
};
#endif
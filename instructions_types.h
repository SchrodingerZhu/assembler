//
// Created by schrodinger on 1/8/20.
//

#ifndef ASSEMBLER_INSTRUCTIONS_TYPES_H
#define ASSEMBLER_INSTRUCTIONS_TYPES_H

#include "global.h"
union Instruction {
    uint32_t __content;
    struct { uint32_t f: 6, S: 5, d: 5, t:5, s: 5, op: 6; } INST_R, Special2;
    struct { uint32_t C: 16, R: 5, s: 5, op: 6; } INST_RI;
    struct { uint32_t op: 6, s: 5, t: 5, C: 16; } INST_I;
    struct { uint32_t op: 6, A: 26; } INST_J;
    struct { uint32_t A: 11, d: 5, t: 5, R: 5, op: 6; } COP;
};
#endif
//
// Created by schrodinger on 1/9/20.
//

#ifndef ASSEMBLER_RI_H
#define ASSEMBLER_RI_H

#include "global.h"
#include "instructions_types.h"
#include "parser.h"
#include <absl/container/flat_hash_map.h>

extern const absl::flat_hash_map<std::string, uint8_t> RIMap;

Instruction generate_RI(const char* inst);

inline Instruction parse_RI() {
    return generate_RI(next_word().data());
}
#endif //ASSEMBLER_RI_H

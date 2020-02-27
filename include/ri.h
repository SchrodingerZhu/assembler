//
// Created by schrodinger on 1/9/20.
//

#ifndef ASSEMBLER_RI_H
#define ASSEMBLER_RI_H

#include "global.h"
#include "instructions_types.h"
#include "parser.h"
#include <absl/container/flat_hash_map.h>
/// Map between RI-Type Instructions' names and their format
extern const absl::flat_hash_map<std::string, uint8_t> RIMap;
/*!
 * Generate a RI-Type Instruction based on the current thread local line.
 * @param inst instruction name
 * @return the instruction value
 */
Instruction generate_RI(const char *inst);

/*!
 * Map between R-Type Instructions' names and their format. (name is auto resolved)
 * @return
 */
FAST_PATH Instruction parse_RI() {
    return generate_RI(next_word().data());
}

#endif //ASSEMBLER_RI_H

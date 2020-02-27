//
// Created by schrodinger on 1/10/20.
//

#ifndef ASSEMBLER_I_H
#define ASSEMBLER_I_H

#include "global.h"
#include "instructions_types.h"
#include <absl/container/flat_hash_map.h>
#include "parser.h"

/*!
 * Represents the order of arguments when parsing.
 */
enum I_ORDER : uint8_t {
    /// fill register t with `0b00000`
    rt0 = 0,
    /// fill register t with `0b00001`
    rt1 = 1,
    /// parse register t from line
    rt_,
    /// parse register s from line
    rs_,
    /// parse immediate value from line
    imm,
    /// parse bracketed register value
    brs,
    /// do nothing
    non,
    /// fill register t with `0b01000`
    rt8 = 8,
    /// fill register t with `0b01001`
    rt9 = 9,
    /// fill register t with `0b01010`
    rta = 0xa,
    /// fill register t with `0b01011`
    rtb = 0xb,
    /// fill register t with `0b01100`
    rt0xc = 0xc,
    /// fill register t with `0b01110`
    rt0xe = 0xe,
    /// fill register t with `0b10000`
    rt0x10 = 0x10,
    /// fill register t with `0b10001`
    rt0x11 = 0x11,
};

/*!
 * The instruction format of a I-Type Instruction.
 */
struct IInstruction {
    /// operation code
    uint8_t opcode : 6;
    /// register parsing order
    I_ORDER order[3]{};
    /*!
     * construct I-Type format
     */
    IInstruction(/*! operation code */uint8_t opcode, /*! register parsing order */std::initializer_list<I_ORDER> order);
};

/*!
 * Solve immediate label address later by pushing the instruction into waiting queue.
 * @attention the pushing operation involves mutex lock
 */
void solve_imm(/*! current instruction content*/ Instruction & inst, /*! instruction name */ const char *name);

/*!
 * Generate an I-Type Instruction based on the current `thread_local` line.
 * @returns the instruction value
 */
Instruction generate_I(/*! instruction name */const char *inst);

/// A map between the names of I-Type Instructions and their parsing format.
extern const absl::flat_hash_map<std::string, IInstruction> IMap;

/*!
 * Generate an I-Type Instruction based on the current `thread_local` line. (Name is auto resolved.)
 * @returns the instruction value
 */
FAST_PATH Instruction parse_I() {
    return generate_I(next_word().data());
}

#endif //ASSEMBLER_I_H

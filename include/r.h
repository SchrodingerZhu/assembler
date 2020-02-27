//
// Created by schrodinger on 1/8/20.
//
/*!
 *
 *
 *
 *
 */
#ifndef ASSEMBLER_R_H
#define ASSEMBLER_R_H

#include "global.h"
#include <absl/container/flat_hash_map.h>
#include "instructions_types.h"
#include "parser.h"

/*!
 *  The place holder of registers
 */
enum Order : uint8_t {
    /// register d
    rd,
    /// register s
    rs,
    /// register t
    rt,
    /// shift amount
    sa,
    /// nothing
    no
};

/*! \brief R-Type Instruction Format
 *
 *  Stands for basic format of R-Type instructions, including register parsing order.
 */
struct RInstruction {
    /// function code
    uint8_t func: 6,
    /// operation code
    op: 6;
    /// register parsing order
    Order order[3]{};

    /*!
     * Create a RInstruction Format
     */
    RInstruction(/*! operation code */ uint8_t op,
            /*! function code */ uint8_t func,
            /*! register parsing order */std::initializer_list<Order> order);
};

/// Map between R-Type Instructions' names and their format
extern const absl::flat_hash_map<std::string, RInstruction> RMap;

/*!
 * Input the Instruction name and generate the instruction based on the content in the `thread_local` line.
 * @returns the instruction value
 */
Instruction generate_R(/*! instruction name */const char *inst);

/*!
 * Special Instruction Generator for JAJR
 * @returns the instruction value
 */
Instruction parse_JALR();

/*!
 * Parse a R-Type Instruction based on the content in the `thread_local` line.
 * @returns the instruction value
 */
Instruction parse_R();

#endif //ASSEMBLER_R_H

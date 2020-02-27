//
// Created by schrodinger on 1/10/20.
//

#ifndef ASSEMBLER_J_H
#define ASSEMBLER_J_H

#include "global.h"
#include "instructions_types.h"
#include "parser.h"
#include <absl/strings/str_cat.h>
/*!
 * parsing a J-Type instruction with the given name
 * @param inst
 * the instruction name
 * @return the instruction value
 */
FAST_PATH Instruction generate_J(const char *inst) {
    if (unlikely(inst[0] != 'j')) {
        throw parse_error(absl::StrCat("not a valid J instruction: ", line));
    }
    if (inst[1] == 0) {
        uint32_t a;
        try {
            a = parse_num<uint32_t>();
        }
        catch (const parse_error &) {
            parser_shared::push_label_queue(counter, parser_shared::J);
            a = 0;
        }
        return Instruction{
                .INST_J = {
                        .A = a >> 2u,
                        .op = 0b000010,
                }
        };
    }
    if (strcmp(inst + 1, "al") == 0) {
        uint32_t a;
        try {
            a = parse_num<uint32_t>();
        }
        catch (const parse_error &) {
            parser_shared::push_label_queue(counter, parser_shared::J);
            a = 0;
        }
        return Instruction{
                .INST_J = {
                        .A = a >> 2u,
                        .op = 0b000011,
                }
        };
    }
    if (unlikely(inst[0] != 'j')) {
        throw parse_error(absl::StrCat("not a valid J instruction: ", line));
    }
    unreachable();
}
/*!
 * parsing a J-Type instruction (the name is auto resolved)
 * @return the instruction value
 */
FAST_PATH Instruction parse_J() {
    return generate_J(next_word().data());
}
/*!
 * check whether the given word is a J-Type Instruction name.
 * @param w
 * the word
 * @return
 * the checking result
 */
FAST_PATH bool is_j(const char *w) {
    return w[0] == 'j' && (w[1] == 0 || (w[1] == 'a' && w[2] == 'l'));
}

#endif //ASSEMBLER_J_H

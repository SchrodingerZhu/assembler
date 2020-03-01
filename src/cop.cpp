//
// Created by schrodinger on 1/9/20.
//

#include <cop.h>

Instruction generate_COP(std::string_view inst) {
    switch (inst[1]) {
        case 'f': {
            auto t = parse_register();
            auto d = parse_register();
            auto sel = parse_next_u8_or_zero();
            return Instruction{
                    .COP = {.A = static_cast<uint32_t>(sel), .d = d, .t = t, .R = 0b00100, .op = 0b010000,}
            };
        }

        case 't': {
            auto t = parse_register();
            auto d = parse_register();
            auto sel = parse_next_u8_or_zero();
            return Instruction{
                    .COP = {.A = static_cast<uint32_t>(sel), .d = d, .t = t, .R = 0b00100, .op = 0b010000,}
            };
        }
        case 'r':
            return ERET;
        default:
            unreachable();
    }
}

//
// Created by schrodinger on 1/8/20.
//

#ifndef ASSEMBLER_GLOBAL_H
#define ASSEMBLER_GLOBAL_H

#include <cstdint>
#include <string_view>
#include <string>
#include <iostream>

#define unreachable()  __builtin_unreachable()
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif //ASSEMBLER_GLOBAL_H

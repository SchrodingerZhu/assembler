//
// Created by schrodinger on 1/8/20.
//

#ifndef ASSEMBLER_GLOBAL_H
#define ASSEMBLER_GLOBAL_H

#include <cstdint>
#include <string_view>
#include <string>
#include <iostream>
#include <memory_resource>
#include <memory>
#include <fstream>
#include <thread>

#define mod std
#define unreachable()  __builtin_unreachable()

#if defined(__GNUC__) || defined(__clang__)
#define unlikely(x)     __builtin_expect((x),0)
#define likely(x)       __builtin_expect((x),1)
#else
#define unlikely(x)     (x)
#define likely(x)       (x)
#endif

#define QUEUE_SIZE (50)
#define BASE_ADDR 0

const size_t WORKERS = std::thread::hardware_concurrency();

extern std::istream *source;
extern std::ostream *result;
extern std::string input;
extern std::string output;

inline void finalize() {
    if (source && source != &std::cin) delete source;
    if (source && result != &std::cout) delete result;
}

#endif //ASSEMBLER_GLOBAL_H

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
#include <CLI11.hpp>
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
#define BASE_ADDR 0x400000

const size_t WORKERS = std::thread::hardware_concurrency();

extern std::istream *source;
extern std::ostream *result;

namespace CLI {
    inline int run(int argc, char **argv) {
        std::string input;
        std::string output;
        App app{"easy MIPS assembler"};
        app.add_option("-f,--file", input, "input path");
        app.add_option("-o,--output", output, "output path");
        CLI11_PARSE(app, argc, argv)
        if (input.empty()) {
            std::cerr << "[WARN] using stdin as input" << std::endl;
            source = &std::cin;
        } else {
            source = new std::ifstream(input);
        }
        if (output.empty()) {
            std::cerr << "[WARN] using stdout as output" << std::endl;
            result = &std::cout;
        } else {
            result = new std::ofstream(output);
        }
        if (!source->good() || !result->good()) {
            throw std::runtime_error{"failed to initialize IO module, perhaps no such file?"};
        }
        return 0;
    }
}

inline void finalize() {
    if (source != &std::cin) delete source;
    if (result != &std::cout) delete result;
}
#endif //ASSEMBLER_GLOBAL_H

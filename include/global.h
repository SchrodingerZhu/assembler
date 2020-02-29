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


/// speed up branch prediction with unreachable condition
#define unreachable()  __builtin_unreachable()
#if defined(__GNUC__) || defined(__clang__)
/// speed up branch prediction with unlikely condition
#define unlikely(x)     __builtin_expect((x),0)
/// speed up branch prediction with likely condition
#define likely(x)       __builtin_expect((x),1)
#else
#define unlikely(x)     (x)
#define likely(x)       (x)
#endif

/// forced inline for fast path functions
#define FAST_PATH static inline __attribute__((always_inline))

/// parallel work pool size
#define QUEUE_SIZE 500

#ifndef BASE_ADDR
/// start address of the output binary
#define BASE_ADDR 0
#endif

/// platform dependent worker pool size
const size_t WORKERS = std::thread::hardware_concurrency();

/// INPUT Device (Default: STDIN)
extern std::istream *source;
/// OUTPUT Device (Default: STDOUT)
extern std::ostream *result;
/// INPUT PATH
extern std::string input;
/// OUTPUT PATH
extern std::string output;

/*!
 * [FAST_PATH] Finalize the IO Devices.
 */
FAST_PATH void finalize() {
    if (source != &std::cin) delete source;
    if (result != &std::cout) delete result;
}

#if __cplusplus <= 201703L

#include <absl/strings/string_view.h>

namespace std {
    using string_view = absl::string_view;
}
#endif
#endif //ASSEMBLER_GLOBAL_H

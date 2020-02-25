//
// Created by schrodinger on 1/8/20.
//

/*
 *
 * GLOBAL_H
 * ---------
 * IO Devices and Useful Macros to Improve the Performance.
 * Global Constants that Will Influence the Final Binary.
 *
 */

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

/*
 *
 * Macro: unreachable/unlikely/likely
 * -----------------------------------
 * Speedup Branch Prediction
 *
 */

#define unreachable()  __builtin_unreachable()
#if defined(__GNUC__) || defined(__clang__)
#define unlikely(x)     __builtin_expect((x),0)
#define likely(x)       __builtin_expect((x),1)
#else
#define unlikely(x)     (x)
#define likely(x)       (x)
#endif


#define FAST_PATH static inline __attribute__((always_inline)) // Adjust GNU inline options

#define QUEUE_SIZE 500 // Parallel Work Pool Size

#ifndef BASE_ADDR
#define BASE_ADDR 0 // Start Address
#endif

const size_t WORKERS = std::thread::hardware_concurrency(); // Platform Dependent Worker Pool Size

extern std::istream *source; // INPUT Device (Default: STDIN)
extern std::ostream *result; // OUTPUT Device (Default: STDOUT)
extern std::string input; // INPUT PATH
extern std::string output; // OUTPUT PATH
/*
 *
 * [FAST_PATH] void finalize()
 * ---------------------------
 * Finalize the IO Devices.
 *
 */
FAST_PATH void finalize() {
    if (source != &std::cin) delete source;
    if (result != &std::cout) delete result;
}

#endif //ASSEMBLER_GLOBAL_H

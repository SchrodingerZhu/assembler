#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
//
// Created by schrodinger on 1/10/20.
//

#ifndef ASSEMBLER_THREAD_PARSER_H
#define ASSEMBLER_THREAD_PARSER_H

#include <parser.h>
#include <r.h>
#include <ri.h>
#include <cop.h>
#include <j.h>
#include <thread>
#include <data.h>
#include <i.h>

/// a flag to represent whether the assembler is in a test mode
extern bool test_flag;

/*!
 * the normal parser life cycle
 * @param finished a global flag to detect whether the process if finished
 */
void parser_life(std::atomic_size_t &finished);

/*!
 * the recovering parser life cycle (recover the instruction for labels)
 */
void parser_recover();

/*!
 * set the test node
 */
void test_mode(bool);

/*!
 * spawn threads and start the parsing
 */
void run_parsing();

/*!
 * generate the data vector
 */
std::vector<char> generate_data();
#endif //ASSEMBLER_THREAD_PARSER_H

#pragma clang diagnostic pop
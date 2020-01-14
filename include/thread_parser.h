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
#include <i.h>

extern bool test_flag;

void parser_life(std::atomic_size_t &finished);

void parser_recover();

void test_mode(bool);

void run_parsing();

#endif //ASSEMBLER_THREAD_PARSER_H

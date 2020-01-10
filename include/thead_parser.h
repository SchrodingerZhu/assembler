//
// Created by schrodinger on 1/10/20.
//

#ifndef ASSEMBLER_THEAD_PARSER_H
#define ASSEMBLER_THEAD_PARSER_H

#include <parser.h>
#include <r.h>
#include <ri.h>
#include <cop.h>
#include <j.h>
#include <i.h>

void parser_life(std::atomic_size_t &finished);

void parser_recover();

void run_parsing();
#endif //ASSEMBLER_THEAD_PARSER_H

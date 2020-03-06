//
// Created by schrodinger on 3/6/20.
//

#ifndef ASSEMBLER_API_H
#define ASSEMBLER_API_H

#include <thread_parser.h>
#include <sstream>

class ErrorStream {
public:
    ErrorStream();

    std::string str();

    ~ErrorStream();

private:
    std::stringstream buffer;
    std::streambuf *old;
};

struct Result {
    std::vector<Instruction> instructions;
    std::vector<char> data_part;
    std::string error_info;
    bool success;

    Result(std::vector<Instruction> instr, std::vector<char> data_part, std::string error_info, bool success);
};

void reset_all();

Result generate_result(std::string path);

#endif //ASSEMBLER_API_H

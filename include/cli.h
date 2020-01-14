//
// Created by schrodinger on 1/14/20.
//

#ifndef ASSEMBLER_CLI_H
#define ASSEMBLER_CLI_H

#include <CLI11.hpp>
#include <bitset>
#include <iostream>
#include <global.h>
#include <thread_parser.h>

namespace CLI {
    inline int run(int argc, char **argv) {
        App app{"easy MIPS assembler"};
        app.add_option("-f,--file", input, "input path");
        app.add_option("-o,--output", output, "output path");
        CLI11_PARSE(app, argc, argv)
        try {
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

            run_parsing();
            if (parser_shared::success) {
                for (auto &i : parser_shared::finished) {
                    (*result) << std::bitset<32>(i.__content) << std::endl;
                }
                if (!output.empty()) {
                    auto file = fopen(absl::StrCat(output, ".bin").data(), "w+");
                    fwrite(parser_shared::finished.data(), 4, parser_shared::finished.size(), file);
                    fclose(file);
                }
            } else {
                std::cerr << "[ERROR] Compilation aborted due to previous errors" << std::endl;
            }
        } catch (const std::runtime_error &exp) {
            std::cerr << "[RUNTIME ERROR] " << exp.what() << std::endl;
        }
        finalize();
        return 0;
    }
}
#endif //ASSEMBLER_CLI_H

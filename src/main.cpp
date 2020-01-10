#include <iostream>
#include <global.h>
#include <thead_parser.h>
#include <bitset>

int main(int argc, char **argv) {
    try {
        CLI::run(argc, argv);
        run_parsing();
        if (parser_shared::success)
            for (auto &i : parser_shared::finished) {
                std::cout << std::bitset<32>(i.__content) << std::endl;
            }
        finalize();
    } catch (const std::runtime_error &exp) {
        std::cerr << "[RUNTIME ERROR] " << exp.what() << std::endl;
    }
}

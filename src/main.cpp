#include <iostream>
#include <global.h>
#include <thead_parser.h>
#include <bitset>
int main(int argc, char** argv) {
    CLI::run(argc, argv);
    run_parsing();
    for (auto &i : parser_shared::finished) {
        std::cout << std::bitset<32>(i.__content) << std::endl;
    }
    finalize();
}

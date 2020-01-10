#include <iostream>
#include <global.h>
#include <thead_parser.h>

int main(int argc, char** argv) {
    CLI::run(argc, argv);
    run_parsing();
    for (auto &i : parser_shared::finished) {
        std::cout << i.__content << std::endl;
    }
    finalize();
}

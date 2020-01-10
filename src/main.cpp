#include <iostream>
#include <global.h>
#include <thead_parser.h>
#include <bitset>

int main(int argc, char **argv) {
    try {
        CLI::run(argc, argv);
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
}

//
// Created by schrodinger on 1/9/20.
//

#include <parser.h>

thread_local mod::string line = {};
thread_local size_t counter = 0;
thread_local size_t address = 0x400000;
parse_error::parse_error(std::string str) : msg(std::move(str)) {}

const char *parse_error::what() const noexcept {
    return msg.c_str();
}

uint8_t parse_register() {
    auto reg_name = next_word();
    if (unlikely(reg_name[0] != '$')) {
        throw parse_error(absl::StrCat("syntax error at: ", reg_name.data()));
    }
    if (unlikely(reg_name[5])) {
        throw parse_error(absl::StrCat("unable to resolve register name: ", reg_name.data()));
    }
    switch (reg_name[1]) {
        case 'v': {
            if (reg_name[2] == '0') CHECK_AND_RETURN(3, 2);
            if (reg_name[2] == '1') CHECK_AND_RETURN(3, 3);
            break;
        }
        case 'a': {
            switch (reg_name[2]) {
                case 't': CHECK_AND_RETURN(3, 1)
                case '0': CHECK_AND_RETURN(3, 4)
                case '1': CHECK_AND_RETURN(3, 5)
                case '2': CHECK_AND_RETURN(3, 6)
                case '3': CHECK_AND_RETURN(4, 7)
            }
            break;
        }
        case 't': {
            if (likely(reg_name[2] >= '0' && reg_name[2] <= '7')) {
                CHECK_AND_RETURN(3, reg_name[2] - '0' + 8)
            }
            if (reg_name[2] == '8') CHECK_AND_RETURN(3, 24)
            if (reg_name[2] == '9') CHECK_AND_RETURN(3, 25)
            break;
        }
        case 's': {
            if (likely(reg_name[2] >= '0' && reg_name[2] <= '7')) {
                CHECK_AND_RETURN(3, reg_name[2] - '0' + 16)
            }
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 29)
            break;
        }
        case 'k': {
            if (reg_name[2] == '0') CHECK_AND_RETURN(3, 26);
            if (reg_name[2] == '1') CHECK_AND_RETURN(3, 27);
            break;
        }
        case 'z': {
            if (std::strcmp(reg_name.data() + 2, "ero") == 0) return 0;
            break;
        }
        case 'g': {
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 28)
            break;
        }
        case 'f': {
            if (reg_name[2] == 'p') CHECK_AND_RETURN(3, 30)
            break;
        }
        case 'r': {
            if (reg_name[2] == 'a') CHECK_AND_RETURN(3, 31)
            break;
        }
    }

    throw parse_error(absl::StrCat("unable to resolve register name: ", reg_name.data()));
}

namespace parser_shared {
    std::size_t global_address {BASE_ADDR};
    mod::vector<task> job_queue {};
    size_t fill_queue() {
        job_queue.clear();
        while (!source->eof() && job_queue.size() < QUEUE_SIZE) {
            mod::string buffer;
            std::getline(*source, buffer);
            mod::string real = cleanup(buffer);
            if (real.empty()) continue;
            job_queue.emplace_back(std::move(real), global_address);
            global_address += 4;
        }
        finished.resize(job_queue.size() + finished.size());
        return job_queue.size();
    }

    mod::vector<Instruction> finished;

    void push_result(Instruction intr, size_t addr) {
        auto index = (addr - BASE_ADDR) / 4;
        finished[index] = intr;
    }

}

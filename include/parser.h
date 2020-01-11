//
// Created by schrodinger on 1/9/20.
//

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <global.h>
#include <iostream>
#include <cstring>
#include <absl/strings/str_cat.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdio>
#include <absl/strings/strip.h>
#include <queue>
#include <optional>
#include <absl/container/flat_hash_map.h>
#include <instructions_types.h>

struct parse_error : std::exception {
    std::string msg;

    explicit parse_error(std::string str);

    [[nodiscard]] const char *what() const noexcept override;
};

namespace parser_shared {
    using task = std::pair<mod::string, size_t>;
    enum RecoverType {
        I, J, RI
    };

    struct recover {
        std::string line;
        size_t addr;
        size_t pos;
        RecoverType type;

        recover(std::string line,
                size_t addr,
                size_t pos,
                RecoverType type);
    };

    extern mod::vector<task> job_queue;
    extern mod::vector<recover> label_queue;
    extern std::mutex error_mutex;
    extern std::mutex label_mutex;
    extern std::mutex label_queue_mutex;
    extern mod::vector<Instruction> finished;

    size_t fill_queue();

    extern std::size_t global_address;
    extern std::atomic_bool success;
    extern absl::flat_hash_map<mod::string, size_t> labels;

    void push_result(const Instruction &intr, size_t addr);

    void push_label_queue(size_t pos, RecoverType type);

    void output_error(const parse_error &error);
}

extern thread_local mod::string line;
extern thread_local size_t counter;
extern thread_local size_t address;

mod::string cleanup(mod::string t);

void eat_whitespace();

bool is_sep(char t);

void eat_sep();

bool at_line_end();

uint8_t peek();

template<class T>
inline T parse_bin() {
    T res = 0;
    while (true) {
        if(line[counter] == '1') {
            res = res * 2 + 1;
        } else if (line[counter] == '0') {
            res = res * 2;
        } else {
            break;
        }
        counter++;
    }
    return res;
}

template <class T>
inline T parse_hex() {
    T res = 0;
    while (true) {
        if(isdigit(line[counter])) {
            res = res * 16 + (line[counter] - '0');
        } else {
            switch (tolower(line[counter])) {
                case 'a': res = res * 16 + 10; break;
                case 'b': res = res * 16 + 11; break;
                case 'c': res = res * 16 + 12; break;
                case 'd': res = res * 16 + 13; break;
                case 'e': res = res * 16 + 14; break;
                case 'f': res = res * 16 + 15; break;
                default: goto OUT;
            }
        }
        counter ++;
    }
OUT:    return res;
}

template <class T>
inline T parse_num() {
    eat_sep();
    if (at_line_end()) {
        throw parse_error("numerical content expected");
    }
    T cur = 0;
    bool flag = false;
    if (line[counter] == '-') {
        ++counter;
        flag = true;
    }
    if (line[counter] == '0' && line[counter + 1] == 'b') {
        counter += 2;
        cur = parse_bin<T>();
    } else if (line[counter] == '0' && line[counter + 1] == 'x') {
        counter += 2;
        cur = parse_hex<T>();
    } else {
        while (!at_line_end() && (isdigit(line[counter]))) {
            auto temp = cur;
            cur = cur * 10 + (line[counter++] - 48);
        }
    }
    if (!is_sep(line[counter]) && line[counter] != '(') {
        throw parse_error("wrong number format");
    }
    return flag ? -cur : cur;
};

uint8_t parse_u8();;

uint8_t parse_next_u8_or_zero();

std::string get_label(size_t pos, std::string_view content);

void parse_label();

std::array<char, 10> next_word();

#define CHECK_AND_RETURN(x, y) {\
if(unlikely(x != 10 && reg_name[x] != 0)) \
{ throw parse_error(absl::StrCat("wrong name of ", reg_name.data())); }\
else return y;}

uint8_t parse_register();

void parser_life();
#endif //ASSEMBLER_PARSER_H

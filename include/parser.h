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
#include <instructions_types.h>
namespace parser_shared {
    using task = std::pair<mod::string, size_t>;
    extern std::mutex resize_mutex;
    extern mod::vector<task> job_queue;
    extern mod::vector<Instruction> finished;
    size_t fill_queue();
    extern std::size_t global_address;
    void push_result(Instruction intr, size_t addr);
}

extern thread_local mod::string line;
extern thread_local size_t counter;
extern thread_local size_t address;

struct parse_error : std::exception {
    std::string msg;
    explicit parse_error(std::string str);
    [[nodiscard]] const char * what() const noexcept override;
};

inline std::string cleanup(mod::string& t) {
    auto a = absl::StripAsciiWhitespace(t);
    if (auto r = std::memchr(a.data(), '#', a.size())) {
        std::string m;
        m.resize((uintptr_t)r - (uintptr_t)a.data());
        std::memcpy(m.data(), a.data(), m.size());
        return m;
    } else {
        return a.data();
    }
}

inline void eat_whitespace() {
    while (counter < line.size() && line[counter] == ' ') counter++;
}

inline bool is_sep(char t) {
    return t <= 32 || t == ',' ;
}

inline void eat_sep() {
    while (counter < line.size() && is_sep(line[counter])) counter++;
}

inline bool at_line_end() {
    return counter == line.size();
}

inline uint8_t peek() {
    eat_whitespace();
    if (unlikely(at_line_end())) {
        throw parse_error("peek from end of line");
    }
    return line[counter];
}

template <class T>
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
            cur = cur * 10 + (line[counter++] - 48);
        }
    }
    return flag ? -cur : cur;
};

inline uint8_t parse_u8() { return parse_num<uint8_t>(); };

inline uint8_t parse_next_u8_or_zero() {
    return (!at_line_end() && (peek() == ',')) ? parse_u8() : 0;
}

inline std::array<char, 10> next_word() {
    std::array<char, 10> buf = {};
    auto a = 0;
    eat_sep();
    if (unlikely(at_line_end())) {
        throw parse_error(absl::StrCat("next word from end of line: ", line, ", position: ", std::to_string(counter)));
    }
    while(!is_sep(line[counter]) && line[counter] != ')') {
        if (unlikely(a == 10)) {
            throw parse_error(absl::StrCat("name is too long: ", line, ", with current buf: ", buf.data()));
        }
        buf[a++] = line[counter++];
    }
    return buf;
}

#define CHECK_AND_RETURN(x, y) {\
if(unlikely(x != 10 && reg_name[x] != 0)) \
{ throw parse_error(absl::StrCat("wrong name of ", reg_name.data())); }\
else return y;}

uint8_t parse_register();

void parser_life();
#endif //ASSEMBLER_PARSER_H

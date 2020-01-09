//
// Created by schrodinger on 1/9/20.
//

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "global.h"
#include <iostream>
#include <cstring>
#include <absl/strings/str_cat.h>

extern thread_local std::string line;
extern thread_local size_t counter;

struct parse_error : std::exception {
    std::string msg;
    explicit parse_error(std::string str);
    [[nodiscard]] const char * what() const noexcept override;
};

inline void eat_whitespace() {
    while (counter < line.size() && line[counter] == ' ') counter++;
}

inline bool is_sep(char t) {
    return !t || t == ' ' || t == ',';
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
inline T parse_num() {
    eat_sep();
    T cur = 0;
    bool flag = 0;
    if (line[counter] == '-') {
        ++counter;
        flag = true;
    }
    while(!at_line_end() && isdigit(line[counter])) {
        cur = cur * 10 + (line[counter++] - 48);
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
        throw parse_error("next word from end of line");
    }
    while(!is_sep(line[counter])) {
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
#endif //ASSEMBLER_PARSER_H

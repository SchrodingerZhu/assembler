//
// Created by schrodinger on 3/6/20.
//

#ifndef ASSEMBLER_DATA_H
#define ASSEMBLER_DATA_H

#include <global.h>
#include <parser.h>
#include <absl/container/flat_hash_map.h>
#include <absl/strings/str_split.h>
#include <absl/container/inlined_vector.h>
#include <array>

enum DATA_TYPE {
    ASCII, ASCIIZ, HALFWORD, WORD, BYTE, SPACE
};

using Data = absl::InlinedVector<char, 16>;

extern absl::flat_hash_map<std::string, DATA_TYPE> data_types;
extern absl::flat_hash_map<char, char> literal_map;

std::string_view check_string(size_t line_count, size_t prefix, std::string_view content);

void fold_string(Data &data, std::string_view literals);

template<class T>
inline T parse_hex(bool flag, size_t line_count, size_t prefix, std::string_view content) {
    auto res = 0;
    for (auto i : content) {
        if (i >= '0' && i <= '9') {
            res = res * 16 + i - '0';
        } else if (i >= 'a' && i <= 'f') {
            res = res * 16 + i - 'a' + 10;
        } else if (i >= 'A' && i <= 'F') {
            res = res * 16 + i - 'A' + 10;
        } else {
            throw parse_error{absl::StrCat("line ", std::to_string(line_count), ", pos ",
                                           std::to_string(prefix), ": wrong hex literal")};
        }
    }
    return flag ? -res : res;
}

template<class T>
inline T parse_bin(bool flag, size_t line_count, size_t prefix, std::string_view content) {
    auto res = 0;
    for (auto i : content) {
        if (i == '0') {
            res = res * 2;
        } else if (i == '1') {
            res = res * 2 + 1;
        } else {
            throw parse_error{absl::StrCat("line ", std::to_string(line_count), ", pos ",
                                           std::to_string(prefix), ": wrong bin literal")};
        }
    }
    return flag ? -res : res;
}

template<class T>
inline T parse_dec(bool flag, size_t line_count, size_t prefix, std::string_view content) {
    auto res = 0;
    for (auto i : content) {
        if (i >= '0' && i <= '9') {
            res = res * 10 + i - '0';
        } else {
            throw parse_error{absl::StrCat("line ", std::to_string(line_count), ", pos ",
                                           std::to_string(prefix), ": wrong dec literal")};
        }
    }
    return flag ? -res : res;
}

template<class T>
void parse_numeric(Data &data, size_t line_count, size_t prefix, std::string_view content) {
    data.resize(sizeof(T));
    auto shift = content.size() > 1 && content.front() == '-';
    if (content.front() == '\'' && content.back() == '\'') {
        Data temp;
        fold_string(temp, content.substr(1, content.size() - 2));
        if (temp.size() != 1) { throw parse_error{"wrong character"}; }
        ::new(data.begin()) T(temp[0]);
    } else if (content.size() > 2 + shift && content[0 + shift] == '0' && content[1 + shift] == 'x') {
        ::new(data.begin()) T(parse_hex<T>(shift, line_count, prefix, content.substr(2 + shift)));
    } else if (content.size() > 2 + shift && content[0 + shift] == '0' && content[1 + shift] == 'b') {
        ::new(data.begin()) T(parse_bin<T>(shift, line_count, prefix, content.substr(2 + shift)));
    } else {
        ::new(data.begin()) T(parse_dec<T>(shift, line_count, prefix, content.substr(shift)));
    }
}

Data solve_line(const parser_shared::data_job &job);


#endif //ASSEMBLER_DATA_H

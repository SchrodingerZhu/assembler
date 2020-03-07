//
// Created by schrodinger on 3/6/20.
//

#include <data.h>
#include <absl/strings/strip.h>

absl::flat_hash_map<std::string, DATA_TYPE> data_types = {
        {".ascii",    ASCII},
        {".asciiz",   ASCIIZ},
        {".halfword", HALFWORD},
        {".word",     WORD},
        {".byte",     BYTE},
        {".space",    SPACE}
};

absl::flat_hash_map<char, char> literal_map = {
        {'n',  '\n'},
        {'\\', '\\'},
        {'b',  '\b'},
        {'t',  '\t'},
        {'a',  '\a'},
        {'?',  '\?'},
        {'"',  '"'},
        {'v',  '\v'},
        {'f',  '\f'},
        {'r',  '\r'},
        {'\'', '\''},
};

void fold_string(Data &data, std::string_view literals) {
    bool flag = false;
    for (size_t i = 0; i < literals.size(); ++i) {
        if (!flag && literals[i] == '\\') {
            flag = true;
            continue;
        }
        if (!flag) {
            data.push_back(literals[i]);
        } else if (literals[i] == 'x' && i + 2 < literals.size()) {
            auto a = literals[i + 1];
            auto b = literals[i + 2];
            int a_value = ('0' <= a && a <= '9') ? a - '0' :
                          ('a' <= a && a <= 'z') ? (a - 'a' + 10) :
                          ('A' <= a && a <= 'Z') ? (a - 'A' + 10) : 16;
            int b_value = ('0' <= b && b <= '9') ? b - '0' :
                          ('a' <= b && b <= 'z') ? (b - 'a' + 10) :
                          ('A' <= b && b <= 'Z') ? (b - 'A' + 10) : 16;
            if (a_value < 16 && b_value < 16) {
                data.push_back(a_value * 16 + b_value);
                i += 2;
                flag = false;
            } else {
                data.push_back('\\');
                data.push_back(literals[i]);
            }
        } else if ('0' <= literals[i] && literals[i] <= '7') {
            int j = i;
            int res = 0;
            while (j < literals.size() && j < i + 3 && '0' <= literals[j] && literals[j] <= '7') {
                auto t = res * 8 + (literals[j] - '0');
                if (t > 0b11111111) break;
                res = t;
                j++;
            }
            i = j - 1;
            data.push_back(res);
            flag = false;
        } else if (literal_map.contains(literals[i])) {
            data.push_back(literal_map[literals[i]]);
            flag = false;
        } else {
            data.push_back('\\');
            data.push_back(literals[i]);
            flag = false;
        }
    }
    if (flag) {
        throw parse_error{"character \\ is not closed"};
    }
}

std::array<std::string_view, 3> split(size_t line_count, std::string_view single_line) {
    int current_start{0};
    int index = 0;
    int in_sep = false;
    int section = 0;
    std::array<std::string_view, 3> result;
    for (int i = 0; i < single_line.size(); ++i) {
        if (in_sep) {
            if (single_line[i] != ' ' && single_line[i] != '\t') {
                in_sep = false;
                current_start = i;
                if (index == 2 && (single_line[i] == '\'' || single_line[i] == '"')) {
                    section = single_line[i];
                }
            } else continue;
        }
        if (index < 2) {
            if (single_line[i] != ' ' && single_line[i] != '\t') continue;
            else {
                result[index] = single_line.substr(current_start, i - current_start);
                in_sep = true;
                index++;
            }
        } else {
            if (!section && (single_line[i] == '\n' || single_line[i] == '\t')) goto ERROR;
            if ((section && single_line[i] != section) || i != single_line.size() - 1)
                continue;
            else {
                result[index] = single_line.substr(current_start, i - current_start + 1);
                in_sep = true;
                index++;
            }
        }
    }
    if (index != 3) {
        ERROR:
        throw parse_error{absl::StrCat("line ", std::to_string(line_count), ": wrong data format")};
    }
    return result;
}

Data solve_line(const parser_shared::data_job &job) {
    int i = 0;
    DATA_TYPE type;
    Data result;
    for (auto &cc : split(job.line_count, job.content)) {
        auto c = absl::StripAsciiWhitespace(cc);
        if (c.empty()) continue;
        i += 1;
        switch (i) {
            case 1:
                if (c.size() < 2 || c.back() != ':') {
                    throw parse_error{absl::StrCat("line ", std::to_string(job.line_count), ", pos ",
                                                   std::to_string(job.prefix), ": expected data label")};
                }
                continue;
            case 2:
                if (!data_types.contains(c)) {
                    throw parse_error{absl::StrCat("line ", std::to_string(job.line_count), ", pos ",
                                                   std::to_string(job.prefix), ": unrecognized type")};
                }
                type = data_types[c];
                break;
            case 3:
                switch (type) {
                    case ASCII: {
                        auto data = check_string(job.line_count, job.prefix, c);
                        fold_string(result, data);
                        result.resize(result.size() + 4 - (result.size() & 3u), 0);
                    }
                        break;
                    case ASCIIZ: {
                        auto data = check_string(job.line_count, job.prefix, c);
                        fold_string(result, data);
                        result.push_back(0);
                        result.resize(result.size() + 4 - (result.size() & 3u), 0);
                    }
                        break;
                    case HALFWORD:
                        parse_numeric<int16_t>(result, job.line_count, job.prefix, c);
                        break;
                    case WORD:
                        parse_numeric<int32_t>(result, job.line_count, job.prefix, c);
                        break;
                    case BYTE:
                        parse_numeric<char>(result, job.line_count, job.prefix, c);
                        break;
                    case SPACE: {
                        parse_numeric<int32_t>(result, job.line_count, job.prefix, c);
                        int32_t number = *reinterpret_cast<int32_t *>(result.begin());
                        if (number <= 0) {
                            throw parse_error{absl::StrCat("line ", std::to_string(job.line_count), ", pos ",
                                                           std::to_string(job.prefix),
                                                           ": space size must be positive")};
                        }
                        result.resize(number, 0);
                    }
                        break;
                }
                break;
            default:
                throw parse_error{absl::StrCat("line ", std::to_string(job.line_count), ", pos ",
                                               std::to_string(job.prefix), ": wrong format")};
        }
    }
    return result;
}

std::string_view check_string(size_t line_count, size_t prefix, std::string_view content) {
    if (content.size() > 2 && content.front() == '"' && content.back() == '"') {
        return content.substr(1, content.size() - 2);
    }
    throw parse_error{absl::StrCat("line ", std::to_string(line_count), ", pos ",
                                   std::to_string(prefix), ": wrong string literal")};
}

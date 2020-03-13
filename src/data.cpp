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

static inline bool separator(char t) {
    return t == ' ' || t == ',' || t == '\t';
}

std::vector<std::string_view> split(size_t line_count, std::string_view single_line) {
    int current_start{0};
    int index = 0;
    bool in_sep = false;
    int section = 0;
    int comma = 0;
    bool flag = false;
    std::vector<std::string_view> split_result;
    for (int i = 0; i < single_line.size(); ++i) {
        if (in_sep) {
            if (!separator(single_line[i])) {
                in_sep = false;
                current_start = i;
                if (index >= 2 && (single_line[i] == '\'' || single_line[i] == '"')) {
                    section = single_line[i];
                    flag = true;
                }
                if (index != 2 && section == '\"') goto ERROR;
            } else {
                if (single_line[i] == ',') comma += 1;
                continue;
            }
        }
        if (index < 2) {
            if (!separator(single_line[i])) continue;
            else if (comma) goto ERROR;
            else {
                split_result.push_back(single_line.substr(current_start, i - current_start));
                in_sep = true;
                comma = (single_line[i] == ',');
                index++;
            }
        } else {
            if (i == single_line.size() - 1 || (!flag && section == single_line[i])) {
                i += 1;
                if (i != single_line.size() && !separator(single_line[i])) goto ERROR;
                goto REAL;
            }
            if (section || !separator(single_line[i])) {
                flag = (single_line[i] == '\\');
                continue;
            } else {
                REAL:
                if ((index == 2 && comma != 0) || (index > 3 && comma != 1)) {
                    goto ERROR;
                }
                split_result.push_back(single_line.substr(current_start, i - current_start));
                in_sep = true;
                flag = false;
                comma = i != single_line.size() && (single_line[i] == ',');
                index++;
                section = 0;
            }
        }
    }
    if (section) {
        ERROR:
        throw parse_error{absl::StrCat("line ", std::to_string(line_count), ": wrong data format")};
    }
    return split_result;
}

Data solve_line(const parser_shared::data_job &job) {
    int i = 0;
    DATA_TYPE type;
    Data line_result;
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
            default:
                switch (type) {
                    case ASCII: {
                        auto data = check_string(job.line_count, job.prefix, c);
                        fold_string(line_result, data);

                    }
                        break;
                    case ASCIIZ: {
                        auto data = check_string(job.line_count, job.prefix, c);
                        fold_string(line_result, data);
                        line_result.push_back(0);

                    }
                        break;
                    case HALFWORD:
                        parse_numeric<int16_t>(line_result, job.line_count, job.prefix, c);
                        break;
                    case WORD:
                        parse_numeric<int32_t>(line_result, job.line_count, job.prefix, c);
                        break;
                    case BYTE:
                        parse_numeric<char>(line_result, job.line_count, job.prefix, c);
                        break;
                    case SPACE: {
                        parse_numeric<int32_t>(line_result, job.line_count, job.prefix, c);
                        int32_t number = *reinterpret_cast<int32_t *>(line_result.begin());
                        if (number <= 0) {
                            throw parse_error{absl::StrCat("line ", std::to_string(job.line_count), ", pos ",
                                                           std::to_string(job.prefix),
                                                           ": space size must be positive")};
                        }
                        line_result.resize(number, 0);
                    }
                        break;
                }
                break;
        }
    }
    if (line_result.size() & 3u)
        line_result.resize(line_result.size() + 4 - (line_result.size() & 3u), 0);
    return line_result;
}

std::string_view check_string(size_t line_count, size_t prefix, std::string_view content) {
    if (content.size() > 2 && content.front() == '"' && content.back() == '"') {
        return content.substr(1, content.size() - 2);
    }
    throw parse_error{absl::StrCat("line ", std::to_string(line_count), ", pos ",
                                   std::to_string(prefix), ": wrong string literal")};
}

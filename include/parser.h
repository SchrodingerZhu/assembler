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

/*!
 * Represents all the expection that may happen during the parsing procedure
 */
struct parse_error : std::exception {
    /// error message
    std::string msg;

    /*!
     * construct a parse_error
     * @param str error message
     */
    explicit parse_error(std::string str);

    [[nodiscard]] const char *what() const noexcept override;
};
/*!
 * All the contents will the shared across parsing threads.
 */
namespace parser_shared {
    /*!
     * Stands for a single line parsing task.
     */
    struct task {
        /// line content
        mod::string content;
        /// line address
        size_t address;
        /// line number
        size_t line_count;
        /// line start position
        size_t prefix;

        /*!
         * construct a single line parsing task
         * @param content line content
         * @param address line address
         * @param line_count line number
         * @param prefix line start position
         */
        task(mod::string content, size_t address, size_t line_count, size_t prefix);
    };

    /*!
     * Stands for error recover type
     */
    enum RecoverType {
        /// Recover a I-Type Instruction
        I,
        /// Recover a J-Type Instruction
        J,
        /// Recover a RI-Type Instruction
        RI
    };

    /*!
     * Stands for recover job (after label resolution)
     */
    struct recover {
        /// line content
        std::string line;
        /// line address
        size_t addr;
        /// recover position
        size_t pos;
        /// recover type
        RecoverType type;
        /// line number
        size_t line_count;
        /// line start position
        size_t prefix;

        /*!
         * construct a recover job
         * @param line line content
         * @param addr line address
         * @param pos line position
         * @param type recover type
         * @param line_count line number
         * @param prefix line start position
         */
        recover(std::string line,
                size_t addr,
                size_t pos,
                RecoverType type,
                size_t line_count, size_t prefix);
    };

    /// parallel job queue
    extern mod::vector<task> job_queue;
    /// parallel label resolution queue
    extern mod::vector<recover> label_queue;
    /// a mutex for stderr
    extern std::mutex error_mutex;
    /// a mutex for label map
    extern std::mutex label_mutex;
    /// a mutex for label resolution queue
    extern std::mutex label_queue_mutex;
    /// finished instructions
    extern mod::vector<Instruction> finished;
    /*!
     * read new lines and add them to the work queue
     * @return the number of new lines
     */
    size_t fill_queue();

    /// a global counter for the line number
    extern std::size_t global_line_count;
    /// a global counter for the current address
    extern std::size_t global_address;
    /// an atomic global flag for the parsing state
    extern std::atomic_bool success;
    /// an SIMD hashmap for all labels
    extern absl::flat_hash_map<mod::string, size_t> labels;
    /*!
     * push the parsed instruction to the current position.
     * @param intr instruction value
     * @param addr instruction line address
     */
    void push_result(const Instruction &intr, size_t addr);
    /*!
     * push a job into recover mode to wait for the label
     * @attention the pushing operations will lock the mutex
     * @param pos current position
     * @param type label recover type
     */
    void push_label_queue(size_t pos, RecoverType type);
    /*!
     * output an error to stderr
     * @attention the output will lock the mutex
     * @param error parse error (with error message)
     */
    void output_error(const parse_error &error, size_t, size_t);
}
/// a thread local line
extern thread_local mod::string line;
/// a thread local counter, acts as the line cursor
extern thread_local size_t counter;
/// thread local line address
extern thread_local size_t address;
/// thread local line number
extern thread_local size_t local_line;
/// thread local line start position
extern thread_local size_t local_prefix;
/*!
 * trim the line and remove all comments
 * @param t line content
 * @return the trimmed line
 */
mod::string cleanup(mod::string t);
/*!
 * eat up all nonsense charactor and ASCII whitespace (by forwarding the line cursor)
 */
void eat_whitespace();
/*!
 * check whether the given character is a separator (comma or whitespace)
 * @param t the character to check
 * @return the checking result
 */
bool is_sep(char t);
/*!
 * eat up all separators (comma or whitespace) (by forwarding the line cursor)
 */
void eat_sep();
/*!
 * check whether we are at the end of the current line
 * @return the checking result
 */
bool at_line_end();
/*!
 * retrieve the current character without increasing the cursor
 */
uint8_t peek();
/*!
 * parse a binary number
 * @attention
 * this ought to be used internally by the parser, since it will not recognize the number format.
 * to parse a number from the line, invoke parse_num() instead
 * @tparam T the type of integer
 * @return the result number
 */
template<class T>
inline T parse_bin() {
    T res = 0;
    while (true) {
        if (line[counter] == '1') {
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
/*!
 * parse a hexdecimal number
 * @attention
 * this ought to be used internally by the parser, since it will not recognize the number format.
 * to parse a number from the line, invoke parse_num() instead
 * @tparam T the type of integer
 * @return the result number
 */
template<class T>
inline T parse_hex() {
    T res = 0;
    while (true) {
        if (isdigit(line[counter])) {
            res = res * 16 + (line[counter] - '0');
        } else {
            switch (tolower(line[counter])) {
                case 'a':
                    res = res * 16 + 10;
                    break;
                case 'b':
                    res = res * 16 + 11;
                    break;
                case 'c':
                    res = res * 16 + 12;
                    break;
                case 'd':
                    res = res * 16 + 13;
                    break;
                case 'e':
                    res = res * 16 + 14;
                    break;
                case 'f':
                    res = res * 16 + 15;
                    break;
                default:
                    goto OUT;
            }
        }
        counter++;
    }
    OUT:
    return res;
}
/*!
 * parse a number, the format will be recognized automatically
 * @tparam T the type of integer
 * @return the result number
 */
template<class T>
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
/*!
 * parse a simple 8-bit integer (used for register name and special immediate values)
 * @return the result number
 */
uint8_t parse_u8();

/*!
 * parse a simple 8-bit integer, return zero on failure (used for register name and special immediate values)
 * @return the result number
 */
uint8_t parse_next_u8_or_zero();

/*!
 * parse the label name
 * @param pos start position of the label
 * @param content line content
 * @return the name of the label
 */
std::string get_label(size_t pos, std::string_view content);

/*!
 * parse the label and automatically register it (in the hashmap)
 */
void parse_label();
/*!
 * parse a lexical word, size limited by 10 (which is obviously enough for the tokens)
 * @return the token
 */
std::array<char, 10> next_word();

/*!
 * a short hand macro for checking the remaining content of the word before retunn the result.
 * this is to discover the error like mis-writing `$v0` as `$v0a`
 */
#define CHECK_AND_RETURN(x, y) {\
if(unlikely(x != 10 && reg_name[x] != 0)) \
{ throw parse_error(absl::StrCat("wrong name of ", reg_name.data())); }\
else return y;}
/*!
 * parse a register name
 * @return the number of the register
 */
uint8_t parse_register();


#endif //ASSEMBLER_PARSER_H

#include <test_utils.h>
#include <data.h>

using namespace parser_shared;

int main() {
    test_case(fold_string_0, {
        Data temp;
        fold_string(temp, "\\xfAf");
        assert_eq(temp.size(), 2);
        auto t = temp[0];
        assert_eq(t, '\xfA');
        t = temp[1];
        assert_eq(t, 'f');
    })
    test_case(fold_string_1, {
        Data temp;
        fold_string(temp, "\\n");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\n');
    })
    test_case(fold_string_2, {
        Data temp;
        fold_string(temp, "\\xff");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\xff');
    })
    test_case(fold_string_3, {
        Data temp;
        fold_string(temp, "\\477");
        assert_eq(temp.size(), 2);
        auto t = temp[0];
        assert_eq(t, '\47');
        t = temp[1];
        assert_eq(t, '7');
    })
    test_case(fold_string_4, {
        Data temp;
        fold_string(temp, "\\f");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\f');
    })
    test_case(fold_string_5, {
        Data temp;
        fold_string(temp, "\\a");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\a');
    })
    test_case(fold_string_6, {
        Data temp;
        fold_string(temp, "\\?");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\?');
    })
    test_case(fold_string_7, {
        Data temp;
        fold_string(temp, "\\\"");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\"');
    })
    test_case(fold_string_8, {
        Data temp;
        fold_string(temp, "\\\\");
        assert_eq(temp.size(), 1);
        auto t = temp[0];
        assert_eq(t, '\\');
    })

    test_case(parse_ascii, {
        auto data = solve_line(data_job{0, 0, "a: .ascii \"123\""});
        assert_eq(data.size(), 4);
        char c = data[0];
        assert_eq(c, '1');
        c = data[1];
        assert_eq(c, '2');
        c = data[2];
        assert_eq(c, '3');
    })
    test_case(parse_asciiz, {
        auto data = solve_line(data_job{0, 0, "a: .asciiz \"123   abc\""});
        assert_eq(data.size(), 12);
        assert_eq(strcmp(data.begin(), "123   abc"), 0);
    })
    test_case(parse_char_A, {
        auto data = solve_line(data_job{0, 0, "a: .byte \t'A'"});
        assert_eq(data.size(), 1);
        auto c = data.front();
        assert_eq(c, 'A');
    })
    test_case(parse_char_B, {
        auto data = solve_line(data_job{0, 0, "a: .byte \t127"});
        assert_eq(data.size(), 1);
        auto c = data.front();
        assert_eq(c, 127);
    })
    test_case(parse_char_C, {
        auto data = solve_line(data_job{0, 0, "a: .byte -127"});
        assert_eq(data.size(), 1);
        auto c = data.front();
        assert_eq(c, -127);
    })
    test_case(parse_char_D, {
        auto data = solve_line(data_job{0, 0, "a: .byte 0b101"});
        assert_eq(data.size(), 1);
        auto c = data.front();
        assert_eq(c, 0b101);
    })
    test_case(parse_char_E, {
        auto data = solve_line(data_job{0, 0, "a: .byte 0x34"});
        assert_eq(data.size(), 1);
        auto c = data.front();
        assert_eq(c, 0x34);
    })
    test_case(parse_halfword_A, {
        auto data = solve_line(data_job{0, 0, "a: .halfword 0x341d"});
        assert_eq(data.size(), 2);
        auto c = *reinterpret_cast<int16_t *>(data.begin());
        assert_eq(c, 0x341d);
    })
    test_case(parse_halfword_B, {
        auto data = solve_line(data_job{0, 0, "a: .halfword -0x341d"});
        assert_eq(data.size(), 2);
        auto c = *reinterpret_cast<int16_t *>(data.begin());
        assert_eq(c, -0x341d);
    })
    test_case(parse_word_A, {
        auto data = solve_line(data_job{0, 0, "a: .word -0x341dffff"});
        assert_eq(data.size(), 4);
        auto c = *reinterpret_cast<int32_t *>(data.begin());
        assert_eq(c, -0x341dffff);
    })
    test_case(parse_word_B, {
        auto data = solve_line(data_job{0, 0, "a: .word 'x'"});
        assert_eq(data.size(), 4);
        auto c = *reinterpret_cast<int32_t *>(data.begin());
        assert_eq(c, 'x');
    })
    test_case(parse_space, {
        auto data = solve_line(data_job{0, 0, "a: .space 12"});
        assert_eq(data.size(), 12);
    })

}
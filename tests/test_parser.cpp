#include "test_utils.h"
#include "../parser.h"
int main() {

    test_case(next_word("add t0, t1"), {
        line = "add t0, t1";
        counter = 0;
        assert_eq(std::string(next_word().data()), "add");
        assert_eq(std::string(next_word().data()), "t0");
        assert_eq(std::string(next_word().data()), "t1");
    })

    test_case(parse_u8, {
        line = "123";
        counter = 0;
        assert_eq(parse_u8(), 123);
    })

    test_case(parse_i8, {
        line = "123";
        counter = 0;
        assert_eq(parse_num<int8_t>(), 123);

        line = "-15";
        counter = 0;
        assert_eq(parse_num<int8_t>(), -15);
    })

    test_case(parse_u32, {
        line = "12345678";
        counter = 0;
        assert_eq(parse_num<uint32_t>(), 12345678);
    })

    TEST_REGISTER("$zero", 0)
    TEST_REGISTER("$zero ", 0)
    TEST_REGISTER("$at", 1)
    TEST_REGISTER("$v0", 2)
    TEST_REGISTER("$v1", 3)
    TEST_REGISTER("$a0", 4)
    TEST_REGISTER("$a1", 5)
    TEST_REGISTER("$a2", 6)
    TEST_REGISTER("$a3", 7)
    TEST_REGISTER("$t0", 8)
    TEST_REGISTER("$t3", 11)
    TEST_REGISTER("$t7", 15)
    TEST_REGISTER("$t8", 24)
    TEST_REGISTER("$t9", 25)
    TEST_REGISTER("$t9,", 25)
    TEST_REGISTER("$k0", 26)
    TEST_REGISTER("$k1", 27)
    TEST_REGISTER("$gp", 28)
    TEST_REGISTER("$sp", 29)
    TEST_REGISTER("$fp", 30)
    TEST_REGISTER("$ra", 31)
    SHOULD_FAIL(TEST_REGISTER("$a4", 0))
    SHOULD_FAIL(TEST_REGISTER("$a2r", 0))
    SHOULD_FAIL(TEST_REGISTER("$t10", 0))
    SHOULD_FAIL(TEST_REGISTER("$miku", 0))
    SHOULD_FAIL(TEST_REGISTER("$nico", 0))
    SHOULD_FAIL(TEST_REGISTER("$zerok", 0))
    return 0;
}
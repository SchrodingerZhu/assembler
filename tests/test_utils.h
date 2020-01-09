//
// Created by schrodinger on 1/9/20.
//

#include <bitset>
#include <iomanip>
#include "../global.h"

#define TEST_REGISTER(name, val) \
    test_case(parse_register(name), { \
    line = name;\
    counter = 0;\
    assert_eq(parse_register(), val);\
})

#define display_bits(real, exp) \
    std::clog << "\n------------\n" << "expected: " << std::setw(8) << std::setfill('0') << std::hex << exp << std::setw(0) << ": " << std::bitset<32>(exp) << std::endl\
              << "output:   "  << std::setw(8) << std::hex << real << std::setw(0) << ": " << std::bitset<32>(real) << std::endl;\


#define test_asm_R(w, e) \
    test_case(w, { \
        line = w; \
        counter = 0; \
        auto res = parse_R().__content; \
        display_bits(res, e); \
        assert_eq(res, e); \
    })\
    std::clog << std::endl;

#define test_asm_RI(w, e) \
    test_case(w, { \
        line = w; \
        counter = 0; \
        auto res = parse_RI().__content; \
        display_bits(res, e); \
        assert_eq(res, e); \
    })\
    std::clog << std::endl;

#define test_asm_COP(w, e) \
    test_case(w, { \
        line = w; \
        counter = 0; \
        auto res = parse_COP().__content; \
        display_bits(res, e); \
        assert_eq(res, e); \
    })\
    std::clog << std::endl;

template <typename T>
class is_displayable
{
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static one test(typeof(&(std::clog << std::declval<C>()))) { return 0; };
    template <typename C> static two test(...) { return {0, 0}; };

public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
    using type = std::integral_constant<bool, value>;
};

#define stringify(name) #name

template <typename T>
void display(const T& m, const T& n, const char*, const char *, std::true_type) {
    std::clog << m << " should be equal to " << n << std::endl;
}

template <typename T>
void display(const T&, const T&, const char * m , const char * n, std::false_type) {
    std::clog << m << " should be equal to " << n << std::endl;
}

#define assert_eq(a, b) {\
    decltype(a) m = (a); \
    decltype(a) n = (b); \
    if (m != n) {\
        display(m, n, stringify(m), stringify(n), is_displayable<decltype(a)>::type {}); \
    }\
    assert((m) == (n)); \
}

#define test_case(name, block) \
    std::clog << "testing "<< stringify(name) << ": ";\
    block\
    std::clog << "[SUCCESS]\n";

#define SHOULD_FAIL(block) \
    try { std::clog << "should fail -> "; block; std::clog << "[FAILED TO FAIL]\n"; } catch(...) {std::clog << "[FAILED SUCCESSFULLY]\n";}

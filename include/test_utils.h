//
// Created by schrodinger on 1/9/20.
//

#include <bitset>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cassert>
#include <parser.h>
#include "global.h"

#define TEST_REGISTER(name, val) \
    test_case(parse_register(name), { \
    line = name;\
    counter = 0;\
    assert_eq(parse_register(), val);\
})

#define display_bits(real, exp) \
    std::clog << "------------\n" << "expected: " << std::setw(8) << std::setfill('0') << std::hex << exp << std::setw(0) << ": " << std::bitset<32>(exp) << std::endl\
              << "output:   "  << std::setw(8) << std::hex << real << std::setw(0) << ": " << std::bitset<32>(real) << std::endl;\

#define test_asm(U, w, e) \
    test_case(w, { \
        line = w; \
        counter = 0; \
        auto res = parse_##U().__content; \
        display_bits(res, e); \
        assert_eq(res, e); \
    })\
    std::clog << std::endl;


#if __cplusplus >= 201709L
template<typename T>
concept Displayable = requires(T a) { std::clog << a; };
#else
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
#endif


#define stringify(name) #name

#if __cplusplus >= 201709L
template <typename T> requires (Displayable<T>)
void display(const T& m, const T& n, const char*, const char *) {
#else
template <typename T>
void display(const T& m, const T& n, const char*, const char *, std::true_type) {
#endif
    std::clog << m << " should be equal to " << n << std::endl;
}

#if __cplusplus >= 201709L
template <typename T> requires (!Displayable<T>)
void display(const T&, const T&, const char * m , const char * n) {
#else
template <typename T>
void display(const T&, const T&, const char * m , const char * n, std::false_type) {
#endif
    std::clog << m << " should be equal to " << n << std::endl;
}
#if __cplusplus >= 201709L
#define assert_eq(a, b) {\
    decltype(a) m = (a); \
    decltype(a) n = (b); \
    if (m != n) {\
        display(m, n, stringify(m), stringify(n)); \
    }\
    assert((m) == (n)); \
}
#else
#define assert_eq(a, b) {\
    decltype(a) m = (a); \
    decltype(a) n = (b); \
    if (m != n) {\
        display(m, n, stringify(m), stringify(n), is_displayable<decltype(a)>::type {}); \
    }\
    assert((m) == (n)); \
}
#endif

#define test_case(name, block) \
    std::clog << "testing "<< stringify(name) << ": \n";\
    block\
    std::clog << "[SUCCESS]\n";

#define SHOULD_FAIL(block) \
    try { std::clog << "should fail -> "; block; std::clog << "[FAILED TO FAIL]\n"; } catch(...) {std::clog << "[FAILED SUCCESSFULLY]\n";}




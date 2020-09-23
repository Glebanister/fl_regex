#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "ctest.h"
#include "regex.h"

#define DEFAULT_CONTEXT_SIZE 1000

#define DEFAULT_CONTEXT_NAME c
#define create()                                                                            \
    regexpr_context_s *DEFAULT_CONTEXT_NAME = create_regexpr_context(DEFAULT_CONTEXT_SIZE); \
    ASSERT_NOT_NULL(c);
#define create_of_size(size)                                                \
    regexpr_context_s *DEFAULT_CONTEXT_NAME = create_regexpr_context(size); \
    ASSERT_NOT_NULL(c);
#define destroy(context) destroy_regexpr_context(DEFAULT_CONTEXT_NAME);

regexpr_s *range(char begin, char end, regexpr_context_s *context) {
    regexpr_s *res = chr(context, begin);
    for (char ch = begin + 1; ch <= end; ++ch) {
        res = alt(context, chr(context, ch), res);
    }
    return res;
}

#define empty() empty(DEFAULT_CONTEXT_NAME)
#define eps() epsilon(DEFAULT_CONTEXT_NAME)
#define chr(r) chr(DEFAULT_CONTEXT_NAME, r)
#define star(r) star(DEFAULT_CONTEXT_NAME, r)
#define seq(l, r) seq(DEFAULT_CONTEXT_NAME, l, r)
#define alt(l, r) alt(DEFAULT_CONTEXT_NAME, l, r)
#define plus(pat) seq(pat, star(pat))
#define range(begin, end) range(begin, end, DEFAULT_CONTEXT_NAME)

#define match(r, s) ASSERT_EQUAL(true, re_match(r, s, DEFAULT_CONTEXT_NAME))
#define mismatch(r, s) ASSERT_EQUAL(false, re_match(r, s, DEFAULT_CONTEXT_NAME))

CTEST(context, creation) {
    create();
    ASSERT_EQUAL(c->last_elem_id, 0);
    ASSERT_EQUAL(c->size, DEFAULT_CONTEXT_SIZE);
    ASSERT_NOT_NULL(c->memory);
    destroy();
}

CTEST(short_regex, chr) {
    create();

    match(chr('a'), "a");

    mismatch(chr('a'), "b");

    destroy();
}

CTEST(short_regex, empty) {
    create();

    match(eps(), "");

    mismatch(eps(), "a");
    mismatch(chr('a'), "");
    mismatch(empty(), "");

    destroy();
}

CTEST(short_regex, star) {
    create();

    match(star(chr('a')), "");
    match(star(chr('a')), "a");
    match(star(star(chr('a'))), "a");
    match(star(star(chr('a'))), "aaaaaaaa");
    match(star(star(chr('a'))), "");
    match(star(chr('a')), "aaaaaaaaaaaaaaaaaaa");

    mismatch(star(chr('a')), "b");
    mismatch(star(chr('a')), "ab");
    destroy();
}

CTEST(short_regex, seq) {
    create();

    match(seq(chr('a'), chr('b')), "ab");
    match(seq(chr('b'), chr('b')), "bb");
    match(seq(eps(), eps()), "");

    mismatch(seq(eps(), eps()), "a");
    mismatch(seq(chr('a'), chr('b')), "ac");
    mismatch(seq(chr('a'), chr('b')), "cb");

    destroy();
}

CTEST(short_regex, alt) {
    create();

    match(alt(chr('a'), chr('b')), "a");
    match(alt(chr('a'), chr('b')), "b");
    match(alt(chr('a'), eps()), "");
    match(alt(chr('a'), eps()), "a");

    mismatch(alt(chr('a'), chr('b')), "c");
    mismatch(alt(chr('a'), chr('b')), "");
    mismatch(alt(eps(), eps()), "asdf");

    destroy();
}

CTEST(medium_regex, alt_star_star) {
    create();

    // a*|b*
    regexpr_s *reg = alt(star(chr('a')), star(chr('b')));

    match(reg, "aaaa");
    match(reg, "bbbb");
    match(reg, "a");
    match(reg, "b");
    match(reg, "");

    mismatch(reg, "ab");
    mismatch(reg, "ba");
    mismatch(reg, "c");
    mismatch(reg, "ac");
    mismatch(reg, "cb");

    destroy();
}

CTEST(medium_regex, seq_chr_star) {
    create();

    // a(b|c)*
    regexpr_s *reg = seq(chr('a'), star(alt(chr('b'), chr('c'))));

    match(reg, "abbbbbbbb");
    match(reg, "acccccc");
    match(reg, "a");
    match(reg, "acbcbbbccccbcbcbcbc");
    match(reg, "ab");
    match(reg, "ac");

    mismatch(reg, "aabcbccbcbcb");
    mismatch(reg, "");
    mismatch(reg, "ax");
    mismatch(reg, "cbbcbcbcbcbc");

    destroy();
}

CTEST(medium_regex, alt_seq_seq) {
    create();

    // a*b*|c*d*
    regexpr_s *in1 = star(chr('a'));
    regexpr_s *in2 = star(chr('b'));
    regexpr_s *in3 = star(chr('c'));
    regexpr_s *in4 = star(chr('d'));

    regexpr_s *reg = alt(seq(in1, in2), seq(in3, in4));

    match(reg, "abbbbbbbb");
    match(reg, "d");
    match(reg, "");
    match(reg, "aaaaabbbbb");
    match(reg, "ccccdddddd");
    match(reg, "aaaaaaaaa");

    mismatch(reg, "ac");
    mismatch(reg, "abababbcbaba");
    mismatch(reg, "abababbcdcdcdcd");
    mismatch(reg, "aaaaacccccccc");

    destroy();
}

CTEST(large_regex, simple_email) {
    create_of_size(1000000);

    // [a-zA-Z0-9-_]+@[a-zA-Z0-9-_]+.[a-zA-Z0-9-_]+ (almost e-mail)

    regexpr_s *lower_latin = range('a', 'z');
    regexpr_s *upper_latin = range('A', 'Z');
    regexpr_s *letter = alt(lower_latin, upper_latin);
    regexpr_s *digit = range('0', '9');
    regexpr_s *s = alt(alt(letter, digit), chr('_'));

    regexpr_s *begin_to_at = seq(plus(s), chr('@'));
    regexpr_s *at_to_dot = seq(plus(s), chr('.'));
    regexpr_s *dot_to_end = plus(s);

    regexpr_s *email = seq(begin_to_at, seq(at_to_dot, dot_to_end));

    match(email, "someone@gmail.com");
    match(email, "someone_with_underscore@main.ru");
    match(email, "a@b.c");

    mismatch(email, "some.one_with_underscore@main.ru");
    mismatch(email, "@main.ru");
    mismatch(email, "someone_with_underscoremain.ru");
    mismatch(email, "someone_with_underscore@mainru");
    mismatch(email, "someone_with_underscore@main.ru.other");
    mismatch(email, "?someone_with_underscore@main.ru");
    mismatch(email, "a@.c");
    mismatch(email, "a@b.");

    destroy();
}

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
}

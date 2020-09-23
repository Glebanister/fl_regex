#include "regex.h"

int main() {
    const size_t len = 3000;
    char *str = malloc(len * sizeof(char));
    memset(str, 'a', len - 1);
    str[len - 1] = '\0';

    regexpr_context_s *c = create_regexpr_context(60000000);

    // (a | b | cb)*

    regexpr_s *reg = star(c,
                          alt(c,
                              alt(c,
                                  chr(c, 'a'),
                                  chr(c, 'b')),
                              seq(c,
                                  chr(c, 'c'),
                                  chr(c, 'b'))));

    printf(re_match(reg, str, c) ? "true\n" : "false\n");

    free(str);

    // char *true_tests[] = {
    //     "",
    //     "a",
    //     "aaaa",
    //     "b",
    //     "bb",
    //     "x",
    // };

    // char *false_tests[] = {
    //     "h",
    //     "av",
    //     "va",
    //     "aafaa",
    //     "bg",
    //     "fbb",
    //     "xt",
    //     "tx",
    // };

    // const size_t true_tests_cnt = sizeof(true_tests) / sizeof(char *);

    // for (size_t i = 0; i < true_tests_cnt; ++i) {
    //     printf(re_match(reg, true_tests[i], c) ? "true\n" : "false\n");
    // }

    // printf("\n");

    // const size_t false_tests_cnt = sizeof(true_tests) / sizeof(char *);

    // for (size_t i = 0; i < false_tests_cnt; ++i) {
    //     printf(re_match(reg, false_tests[i], c) ? "true\n" : "false\n");
    // }

    destroy_regexpr_context(c);
    return 0;
}

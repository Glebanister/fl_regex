#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "regex.h"

const uint64_t MAX_BYTES = 8ULL * 1024ULL * 1024ULL * 1024ULL; // 8 GB

typedef struct mem_info_t {
    void *mem;
    uint64_t size;
} mem_info_t;

mem_info_t malloc_max(void) {
    uint64_t cur_try = MAX_BYTES * 2;
    void *mem = NULL;
    while (cur_try && !mem) {
        cur_try >>= 1;
        mem = malloc(cur_try);
    }
    mem_info_t result = {mem, cur_try};
    return result;
}

int main(int argc, char **argv) {
    size_t string_len = 5e7;
    if (argc > 1) {
        string_len = atoi(argv[1]);
    }

    char *str = (char *)malloc((string_len + 1) * sizeof(char));
    if (!str) {
        printf("can not allocate memory");
        return 0;
    }
    for (size_t i = 0; i < string_len; ++i) {
        str[i] = 'a';
    }
    str[string_len] = '\0';

    mem_info_t memory = malloc_max();
    if (!memory.mem) {
        printf("can not allocate memory");
        free(str);
        return 0;
    }

    regexpr_context_s c;
    c.size = memory.size / sizeof(regexpr_s);
    c.memory = memory.mem;

    printf("memory allocated: %lld megabytes\n", (long long)memory.size / 1024 / 1024);
    printf("expression: (a | b | cb)*\nstring: 'aaa....'\n");
    printf("testing on string of size %lld k\n", (long long)string_len / 1000);
    time_t start_time = clock();

    // (a | b | cb)*

    regexpr_s *reg = star(&c,
                          alt(&c,
                              alt(&c,
                                  chr(&c, 'a'),
                                  chr(&c, 'b')),
                              seq(&c,
                                  chr(&c, 'c'),
                                  chr(&c, 'b'))));

    re_match(reg, str, &c);

    time_t end_time = clock();

    printf("time: %f s\n", (float)(end_time - start_time) / 1000000.f);
    free(memory.mem);
    free(str);
}

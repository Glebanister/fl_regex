#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "regex.h"

const uint64_t DEFAULT_MAX_BYTES = 2ULL * 1024ULL; // 2 GB
const uint64_t DEFAULT_STRING_LEN = 1e4;           // 10 M symbols

typedef struct mem_info_t {
    void *mem;
    uint64_t size;
} mem_info_t;

mem_info_t malloc_max(uint64_t max_bytes) {
    uint64_t cur_try = max_bytes * 2;
    void *mem = NULL;
    while (cur_try && !mem) {
        cur_try >>= 1;
        mem = malloc(cur_try);
    }
    mem_info_t result = {mem, cur_try};
    return result;
}

int main(int argc, char **argv) {
    uint64_t string_len = DEFAULT_STRING_LEN;
    uint64_t max_mem = DEFAULT_MAX_BYTES;

    if (argc > 1) {
        string_len = atoi(argv[1]);
    }
    if (argc > 2) {
        max_mem = atoi(argv[2]);
    }
    string_len *= 1000ULL;
    max_mem *= 1024UL * 1024;

    char *str = (char *)malloc((string_len + 1) * sizeof(char));
    if (!str) {
        printf("can not allocate memory");
        return 0;
    }
    for (size_t i = 0; i < string_len; ++i) {
        str[i] = 'a';
    }
    str[string_len] = '\0';

    mem_info_t memory = malloc_max(max_mem);
    if (!memory.mem) {
        printf("can not allocate memory");
        free(str);
        return 0;
    }

    regexpr_context_s c;
    c.size = memory.size / sizeof(regexpr_s);
    c.memory = memory.mem;
    c.last_elem_id = 0;

    printf("avaiable memory: %.3f GB\n", (float)memory.size / (1024.f * 1024.f * 1024.f));

    // (a | a)*
    {
        printf("\nexpression: (a | a)*\nstring: 'aaa....' (%.2f M symbols)\n", (float)string_len / 1000000.f);
        time_t start_time = clock();
        regexpr_s *reg = star(&c, alt(&c, chr(&c, 'a'), chr(&c, 'a')));
        re_match(reg, str, &c);
        time_t end_time = clock();
        printf("time: %.2f s\n", (float)(end_time - start_time) / 1000000.f);
        printf("memory used: %.3f GB\n", (float)(c.last_elem_id) * sizeof(regexpr_s) / 1024.f / 1024.f / 1024.f);
    }

    c.last_elem_id = 0;

    // a*
    {
        printf("\nexpression: a*\nstring: 'aaa....' (%.2f M symbols)\n", (float)string_len / 1000000.f);
        time_t start_time = clock();
        regexpr_s *reg = star(&c, chr(&c, 'a'));
        re_match(reg, str, &c);
        time_t end_time = clock();
        printf("time: %.2f s\n", (float)(end_time - start_time) / 1000000.f);
        printf("memory used: %.3f GB\n", (float)(c.last_elem_id) * sizeof(regexpr_s) / 1024.f / 1024.f / 1024.f);
    }
    free(memory.mem);
    free(str);
}

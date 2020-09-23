#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum regexpr_error {
    ERR_NO,
    ERR,
    ERR_NULL_POINTER,
};

void fail(const char *message);

enum regexpr {
    REG_EMPTY,
    REG_EPSILON,
    REG_CHAR,
    REG_STAR,
    REG_SEQ,
    REG_ALT,
};

#pragma pack(push)
#pragma pack(1)
typedef struct regexpr_s {
    int type;
    struct regexpr_s *first;
    struct regexpr_s *second;
    char ch;
} regexpr_s;
#pragma pack(pop)

typedef struct regexpr_context_s {
    size_t size;
    size_t last_elem_id;
    regexpr_s *memory;
} regexpr_context_s;

regexpr_context_s *create_regexpr_context(size_t size);
void destroy_regexpr_context(regexpr_context_s *context);

regexpr_s *create_regex_in_context(regexpr_context_s *context);

bool re_nullable(regexpr_s *reg);
regexpr_s *re_derivative(char c, regexpr_s *reg, regexpr_context_s *context);
bool re_match(regexpr_s *reg, const char *str, regexpr_context_s *context);

regexpr_s *empty(regexpr_context_s *context);
regexpr_s *epsilon(regexpr_context_s *context);
regexpr_s *chr(regexpr_context_s *context, char ch);
regexpr_s *seq(regexpr_context_s *context, regexpr_s *l, regexpr_s *r);
regexpr_s *alt(regexpr_context_s *context, regexpr_s *l, regexpr_s *r);
regexpr_s *star(regexpr_context_s *context, regexpr_s *r);

#endif // REGEX_H

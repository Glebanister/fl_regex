#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"

void fail(const char *message) {
    printf("%s\n", message);
    exit(1);
}

regexpr_context_s *create_regexpr_context(size_t size) {
    regexpr_s *memory = malloc(size * sizeof(regexpr_s));
    if (memory == NULL) {
        fail("can not allocate memory");
        return NULL;
    }
    regexpr_context_s *new_context = malloc(sizeof(regexpr_context_s));
    new_context->memory = memory;
    new_context->size = size;
    new_context->last_elem_id = 0;
    return new_context;
}

regexpr_s *create_regex_in_context(regexpr_context_s *context) {
    if (!context) {
        return NULL;
    }
    if (context->last_elem_id == context->size) {
        fail("regex context overflow");
        return NULL;
    }
    return context->memory + (context->last_elem_id++);
}

bool re_nullable(regexpr_s *reg) {
    if (!reg) {
        exit(1);
        return false; // TODO: error list
    }

    switch (reg->type) {
    case REG_EMPTY:
    case REG_CHAR:
        return false;

    case REG_EPSILON:
    case REG_STAR:
        return true;

    case REG_ALT: {
        return re_nullable(reg->first) || re_nullable(reg->second);
    }

    case REG_SEQ: {
        return re_nullable(reg->first) && re_nullable(reg->second);
    }
    }
    return false;
}

regexpr_s *empty(regexpr_context_s *context) {
    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_EMPTY;
    return new_reg;
}

regexpr_s *epsilon(regexpr_context_s *context) {
    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_EPSILON;
    return new_reg;
}

regexpr_s *chr(regexpr_context_s *context, char ch) {
    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_CHAR;
    new_reg->ch = ch;
    return new_reg;
}

regexpr_s *seq(regexpr_context_s *context,
               regexpr_s *l,
               regexpr_s *r) {
    if ((l && l->type == REG_EPSILON) || (r && r->type == REG_EMPTY)) {
        return r;
    }
    if ((r && r->type == REG_EPSILON) || (l && l->type == REG_EMPTY)) {
        return l;
    }

    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_SEQ;
    new_reg->first = l;
    new_reg->second = r;
    return new_reg;
}

regexpr_s *alt(regexpr_context_s *context,
               regexpr_s *l,
               regexpr_s *r) {

    if (l->type == REG_EMPTY) {
        return r;
    }
    if (r->type == REG_EMPTY) {
        return l;
    }

    if (l->type == REG_EPSILON) {
        return re_nullable(r) ? r : alt(context, l, r);
    }

    if (r->type == REG_EPSILON) {
        return re_nullable(l) ? l : alt(context, r, l);
    }

    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_ALT;
    new_reg->first = l;
    new_reg->second = r;
    return new_reg;
}

regexpr_s *star(regexpr_context_s *context,
                regexpr_s *r) {

    if (r) {
        switch (r->type) {
        case REG_EMPTY:
            return epsilon(context);
        case REG_EPSILON:
            return r;
            break;
        case REG_STAR:
            return r->first;

        default:
            break;
        }
    }

    regexpr_s *new_reg = create_regex_in_context(context);
    if (!new_reg) {
        return NULL;
    }
    new_reg->type = REG_STAR;
    new_reg->first = r;
    return new_reg;
}

void destroy_regexpr_context(regexpr_context_s *context) {
    if (context) {
        free(context->memory);
        free(context);
    }
}

regexpr_s *re_derivative(char c, regexpr_s *reg, regexpr_context_s *context) {
    switch (reg->type) {
    case REG_EMPTY:
    case REG_EPSILON:
        return empty(context);
        break;

    case REG_CHAR: {
        if (reg->ch == c) {
            return epsilon(context);
        } else {
            return empty(context);
        }
    }

    case REG_ALT: {
        return alt(context,
                   re_derivative(c, reg->first, context),
                   re_derivative(c, reg->second, context));
    }

    case REG_SEQ: {
        regexpr_s *p = reg->first;
        regexpr_s *q = reg->second;
        if (re_nullable(p)) {
            return alt(context,
                       seq(context, re_derivative(c, p, context), q),
                       re_derivative(c, q, context));
        } else {
            return seq(context, re_derivative(c, p, context), q);
        }
    }

    case REG_STAR: {
        regexpr_s *r = reg->first;
        return seq(context, re_derivative(c, r, context), star(context, r));
    }
    }
    return NULL;
}

bool re_match(regexpr_s *reg, const char *str, regexpr_context_s *context) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
        reg = re_derivative(str[i], reg, context);
    }
    return re_nullable(reg);
}

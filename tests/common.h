#ifndef _LETMECREATE_TESTS_COMMON_H
#define _LETMECREATE_TESTS_COMMON_H

#include <stdbool.h>
#include "core/common.h"

struct test_case {
    char name[MAX_STR_LENGTH];
    bool(*test_func)(void);
};

struct test {
    char name[MAX_STR_LENGTH];
    unsigned int case_cnt;
    struct test_case *cases;
};

int run_test(struct test test);

#endif

#ifndef _LETMECREATE_TESTS_COMMON_H
#define _LETMECREATE_TESTS_COMMON_H

#include <stdbool.h>
#include <letmecreate/core/common.h>

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

void sleep_ms(unsigned int duration);

/* timeout in seconds */
int wait_for_switch(unsigned int timeout);

/* timeout in seconds */
int ask_question(const char *question, unsigned int timeout);

#define ADD_TEST_CASE(TEST, CASE) \
    do { \
        struct test_case *tmp = test_##TEST.cases; \
        while (tmp->name[0] != 0) \
            ++tmp; \
        struct test_case case_##CASE = { #CASE, test_##TEST##_##CASE }; \
        *tmp = case_##CASE; \
    } while(0)

#define CREATE_TEST(NAME, CASE_CNT) \
    struct test test_##NAME = { \
        .name = #NAME, \
        .case_cnt = CASE_CNT, \
        .cases = malloc((CASE_CNT) * sizeof(struct test_case)) \
    }; \
    memset(test_##NAME.cases, 0, (CASE_CNT) * sizeof(struct test_case));

#endif

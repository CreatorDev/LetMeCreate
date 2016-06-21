#include <assert.h>
#include <stdio.h>
#include "common.h"

int run_test(struct test test)
{
    unsigned int i;
    unsigned int test_passed_cnt = 0;

    assert(test.name != NULL);
    assert(test.cases != NULL);
    assert(test.case_cnt != 0);

    printf("----------- %s -----------\n\n", test.name);

    for (i = 0; i < test.case_cnt; ++i) {
        printf("%2u/%2u\t[%s]\n", i+1, test.case_cnt, test.cases[i].name);

        if (test.cases[i].test_func()) {
            ++test_passed_cnt;
            printf("PASS\n");
        } else {
            printf("FAIL\n");
        }
    }

    printf("\nresult: ");
    if (test_passed_cnt == test.case_cnt)
        printf("all tests passed\n");
    else if (test_passed_cnt == 0)
        printf("all tests failed\n");
    else
        printf("%2u/%2u tests passed\n", test_passed_cnt, test.case_cnt);

    if (test_passed_cnt != test.case_cnt)
        return -1;

    return 0;
}

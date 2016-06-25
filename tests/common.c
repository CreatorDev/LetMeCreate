#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "common.h"
#include "core/switch.h"

static volatile uint8_t switch_status;

int run_test(struct test test)
{
    unsigned int i;
    unsigned int test_passed_cnt = 0;

    assert(test.name != NULL);
    assert(test.cases != NULL);
    assert(test.case_cnt != 0);

    printf("----------- %s -----------\n\n", test.name);

    for (i = 0; i < test.case_cnt; ++i) {
        printf("%2u/%u\t[%s]\n", i+1, test.case_cnt, test.cases[i].name);

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
        printf("%2u/%u tests passed\n", test_passed_cnt, test.case_cnt);

    if (test_passed_cnt != test.case_cnt)
        return -1;

    return 0;
}

static void switch_callback(void)
{
    ++switch_status;
}

int wait_for_switch(unsigned int timeout)
{
    if (switch_init() < 0)
        return -1;

    switch_status = 0;
    if (switch_add_callback(SWITCH_ALL_EVENTS, switch_callback) < 0)
        return -1;

    timeout *= 1000;
    while (switch_status < 2 && timeout > 0) {
        sleep_ms(50);
        timeout -= 50;
    }

    if (switch_release() < 0)
        return -1;

    if (timeout == 0) {
        printf("Timeout\n");
        return -1;
    }

    return 0;
}

void sleep_ms(unsigned int duration)
{
    struct timespec rem, req;
    unsigned int secs_cnt = duration / 1000;
    duration -= secs_cnt * 1000;
    req.tv_sec = secs_cnt;
    req.tv_nsec = duration * 1000000;

    while (nanosleep(&req, &rem))
        req = rem;
}

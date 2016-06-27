/**
 * @brief Implement section 1 of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "core/switch.h"

#define TEST_SWITCH_CASE_CNT        (10)

static volatile uint8_t switch_status = 0;

static void sleep_50ms(void)
{
    struct timespec rem;
    struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 50000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

static void switch_1_pressed(void)
{
    switch_status = SWITCH_1_PRESSED;
}

static void switch_1_released(void)
{
    switch_status = SWITCH_1_RELEASED;
}

static void switch_2_pressed(void)
{
    switch_status = SWITCH_2_PRESSED;
}

static void switch_2_released(void)
{
    switch_status = SWITCH_2_RELEASED;
}

static bool test_switch_add_callback_before_init(void)
{
    return switch_add_callback(0xF, switch_1_pressed) == -1;
}

static bool test_switch_init(void)
{
    return switch_init() == 0;
}

static bool test_switch_add_callback_invalid_mask(void)
{
    return switch_add_callback(0x0, switch_1_pressed) == -1;
}

static bool test_switch_add_callback_null_func(void)
{
    return switch_add_callback(0xF, NULL) == -1;
}

static bool test_switch_remove_callback_invalid_id(void)
{
    return switch_remove_callback(-1) == -1;
}

static bool test_switch_add_remove_callback(void)
{
    uint8_t event = 0;

    for (event = 0x1; event <= 0x8; event <<= 1) {
        int id = -1;
        unsigned int timeout = 10000; /* 10 000ms */
        switch_status = 0;
        switch (event) {
        case SWITCH_1_PRESSED:
            id = switch_add_callback(event, switch_1_pressed);
            break;
        case SWITCH_1_RELEASED:
            id = switch_add_callback(event, switch_1_released);
            break;
        case SWITCH_2_PRESSED:
            id = switch_add_callback(event, switch_2_pressed);
            break;
        case SWITCH_2_RELEASED:
            id = switch_add_callback(event, switch_2_released);
            break;
        }

        if (id < 0)
            return false;

        switch (event) {
        case SWITCH_1_PRESSED:
            printf("Press switch 1\n");
            break;
        case SWITCH_2_PRESSED:
            printf("Press switch 2\n");
            break;
        }

        while(switch_status == 0 && timeout > 0) {
            sleep_50ms();
            timeout -= 50;
        }
        if (timeout == 0) {
            printf("Timeout\n");
            return false;
        }

        if (switch_status != event)
            return false;

        if (switch_remove_callback(id) < 0)
            return false;

        if (switch_remove_callback(id) != -1)
            return false;
    }

    return true;
}

static bool test_switch_release(void)
{
    return switch_release() == 0;
}

static bool test_switch_init_after_release(void)
{
    return switch_init() == 0;
}

static bool test_switch_release_twice(void)
{
    return switch_release() == 0
        && switch_release() == 0;
}

int main(void)
{
    struct test test_switch = {
        .name = "switch",
        .case_cnt = TEST_SWITCH_CASE_CNT,
        .cases = malloc(TEST_SWITCH_CASE_CNT * sizeof(struct test_case))
    };

    /* 1.1 */
    struct test_case add_callback_before_init = { "add callback before init", test_switch_add_callback_before_init };
    test_switch.cases[0] = add_callback_before_init;

    /* 1.2 */
    struct test_case init = { "init", test_switch_init };
    test_switch.cases[1] = init;

    /* 1.3 */
    struct test_case init_twice = { "init twice", test_switch_init };
    test_switch.cases[2] = init_twice;

    /* 1.4 */
    struct test_case add_callback_invalid_mask = { "add callback invalid mask", test_switch_add_callback_invalid_mask };
    test_switch.cases[3] = add_callback_invalid_mask;

    /* 1.5 */
    struct test_case add_callback_null_func = { "add callback null func", test_switch_add_callback_null_func };
    test_switch.cases[4] = add_callback_null_func;

    /* 1.6 */
    struct test_case remove_callback_invalid_id = { "remove callback invalid id", test_switch_remove_callback_invalid_id };
    test_switch.cases[5] = remove_callback_invalid_id;

    /* 1.7 */
    struct test_case add_remove_callback = { "add/remove callback", test_switch_add_remove_callback };
    test_switch.cases[6] = add_remove_callback;

    /* 1.8 */
    struct test_case release = { "release", test_switch_release };
    test_switch.cases[7] = release;

    /* 1.9 */
    struct test_case init_after_release = { "init after release", test_switch_init_after_release };
    test_switch.cases[8] = init_after_release;

    /* 1.10 */
    struct test_case release_twice = { "release twice", test_switch_release_twice };
    test_switch.cases[9] = release_twice;

    int ret = run_test(test_switch);
    free(test_switch.cases);

    return ret;
}

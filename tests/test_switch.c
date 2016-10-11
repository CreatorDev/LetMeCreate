/**
 * @brief Implement SWITCH section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include <letmecreate/core/switch.h>

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
    return switch_init() == 0
        && switch_init() == 0;
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
    return switch_release() == 0
        && switch_release() == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(switch, 9)
    ADD_TEST_CASE(switch, add_callback_before_init);
    ADD_TEST_CASE(switch, init);
    ADD_TEST_CASE(switch, add_callback_invalid_mask);
    ADD_TEST_CASE(switch, add_callback_null_func);
    ADD_TEST_CASE(switch, remove_callback_invalid_id);
    ADD_TEST_CASE(switch, add_remove_callback);
    ADD_TEST_CASE(switch, release);
    ADD_TEST_CASE(switch, init);
    ADD_TEST_CASE(switch, release);

    ret = run_test(test_switch);
    free(test_switch.cases);

    return ret;
}

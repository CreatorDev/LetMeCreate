/**
 * @brief Implement section 4 of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "core/led.h"

#define TEST_LED_CASE_CNT        (23)

static bool test_led_switch_on_off_before_init(void)
{
    return led_switch_on(ALL_LEDS) == -1
        && led_switch_off(ALL_LEDS) == -1;
}

static bool test_led_set_before_init(void)
{
    return led_set(ALL_LEDS, 0xFF) == -1;
}

static bool test_led_set_delay_before_init(void)
{
    return led_set_delay(ALL_LEDS, 500, 500) == -1;
}

static bool test_led_release_before_init(void)
{
    return led_release() == 0;
}

static bool test_led_init(void)
{
    int i;

    if (led_init() < 0)
        return false;

    for (i = 1; i < 0x100; i <<= 1) {
        uint8_t mode;
        if (led_get_mode(i, &mode) < 0)
            return false;
        if (mode != ON_OFF_MODE)
            return false;
    }

    return ask_question("Are all LED's off ?", 30) == 1;
}

static bool test_led_release(void)
{
    if (led_release() < 0)
        return false;

    return ask_question("Are all LED's off ?", 30) == 1;
}

static bool test_led_switch_on(void)
{
    int i;

    if (led_init() < 0)
        return false;

    for (i = 1; i < 0x100; i <<= 1) {
        if (led_switch_on(i) < 0)
            return false;
        sleep_ms(200);
    }

    return ask_question("Did you see all LED's switching on one after the other ?", 30) == 1;
}

static bool test_led_switch_off(void)
{
    int i;

    for (i = 1; i < 0x100; i <<= 1) {
        if (led_switch_off(i) < 0)
            return false;
        sleep_ms(200);
    }

    return ask_question("Did you see all LED's switching off one after the other ?", 30) == 1;
}

static bool test_led_pattern_1(void)
{
    if (led_set(ALL_LEDS, 0xAA) < 0)
        return false;

    return ask_question("Do you see this pattern 01010101 on the LED's ?", 30) == 1;
}

static bool test_led_pattern_2(void)
{
    if (led_set(ALL_LEDS, 0x55) < 0)
        return false;

    return ask_question("Do you see this pattern 10101010 on the LED's ?", 30) == 1;
}

static bool test_led_set_delay_on_off_mode(void)
{
    return led_set_delay(ALL_LEDS, 100, 100) == -1;
}

static bool test_led_timer_mode(void)
{
    int i;

    if (led_configure_timer_mode(ALL_LEDS) < 0)
        return false;

    for (i = 1; i < 0x100; i <<= 1) {
        uint8_t mode = 0;
        if (led_get_mode(i, &mode) < 0)
            return false;
        if (mode != TIMER_MODE)
            return false;
    }

    return ask_question("Are all LED's off ?", 30) == 1;
}

static bool test_led_get_mode_invalid_mode(void)
{
    uint8_t mode;
    return led_get_mode(0x3, &mode) == -1;
}

static bool test_led_get_mode_null_buffer(void)
{
    return led_get_mode(LED_0, NULL) == -1;
}

static bool test_led_set_delay_slow(void)
{
    if (led_set_delay(ALL_LEDS, 500, 500) < 0)
        return false;

    return ask_question("Are all LED's blinking at 1Hz ?", 30) == 1;
}

static bool test_led_set_delay_fast(void)
{
    if (led_set_delay(ALL_LEDS, 100, 1000) < 0)
        return false;

    return ask_question("Are all LED's blinking fast at 1Hz ?", 30) == 1;
}

static bool test_led_switch_on_off_timer_mode(void)
{
    return led_switch_on(ALL_LEDS) == -1
        && led_switch_off(ALL_LEDS) == -1;
}

static bool test_led_set_in_timer_mode(void)
{
    return led_set(ALL_LEDS, 0xAA) == -1;
}

static bool test_led_configure_on_off_mode(void)
{
    int i;

    if (led_configure_on_off_mode(ALL_LEDS) < 0)
        return false;

    for (i = 1; i < 0x100; i <<= 1) {
        uint8_t mode = 0;
        if (led_get_mode(i, &mode) < 0)
            return false;
        if (mode != ON_OFF_MODE)
            return false;
    }

    return ask_question("Are all LED's off ?", 30) == 1;
}

int main(void)
{
    int ret;
    struct test test_led = {
        .name = "led",
        .case_cnt = TEST_LED_CASE_CNT,
        .cases = malloc(TEST_LED_CASE_CNT * sizeof(struct test_case))
    };

    /* 4.1 */
    struct test_case switch_on_off_before_init = { "switch on/off before init", test_led_switch_on_off_before_init };
    test_led.cases[0] = switch_on_off_before_init;

    /* 4.2 */
    struct test_case set_before_init = { "set on/off before init", test_led_set_before_init };
    test_led.cases[1] = set_before_init;

    /* 4.3 */
    struct test_case set_delay_before_init = { "set delay before init", test_led_set_delay_before_init };
    test_led.cases[2] = set_delay_before_init;

    /* 4.4 */
    struct test_case release_before_init = { "release before init", test_led_release_before_init };
    test_led.cases[3] = release_before_init;

    /* 4.5 */
    struct test_case init = { "init", test_led_init };
    test_led.cases[4] = init;

    /* 4.6 */
    struct test_case init_twice = { "init twice", test_led_init };
    test_led.cases[5] = init_twice;

    /* 4.7 */
    struct test_case release = { "release", test_led_release };
    test_led.cases[6] = release;

    /* 4.8 */
    struct test_case release_twice = { "release twice", test_led_release };
    test_led.cases[7] = release_twice;

    /* 4.9 */
    struct test_case switch_on = { "switch on", test_led_switch_on };
    test_led.cases[8] = switch_on;

    /* 4.10 */
    struct test_case switch_off = { "switch off", test_led_switch_off };
    test_led.cases[9] = switch_off;

    /* 4.11 */
    struct test_case pattern_1 = { "pattern 1", test_led_pattern_1 };
    test_led.cases[10] = pattern_1;

    /* 4.12 */
    struct test_case pattern_2 = { "pattern 2", test_led_pattern_2 };
    test_led.cases[11] = pattern_2;

    /* 4.13 */
    struct test_case set_delay_on_off_mode = { "set delay in on/off mode", test_led_set_delay_on_off_mode };
    test_led.cases[12] = set_delay_on_off_mode;

    /* 4.14 */
    struct test_case timer_mode = { "timer mode", test_led_timer_mode };
    test_led.cases[13] = timer_mode;

    /* 4.15 */
    struct test_case get_mode_invalid_mode = { "get mode invalid index", test_led_get_mode_invalid_mode };
    test_led.cases[14] = get_mode_invalid_mode;

    /* 4.16 */
    struct test_case get_mode_null_buffer = { "get mode null buffer", test_led_get_mode_null_buffer };
    test_led.cases[15] = get_mode_null_buffer;

    /* 4.17 */
    struct test_case set_delay_slow = { "set delay slow", test_led_set_delay_slow };
    test_led.cases[16] = set_delay_slow;

    /* 4.18 */
    struct test_case set_delay_fast = { "set delay fast", test_led_set_delay_fast };
    test_led.cases[17] = set_delay_fast;

    /* 4.19 */
    struct test_case switch_on_off_timer_mode = { "switch on/off in timer mode", test_led_switch_on_off_timer_mode };
    test_led.cases[18] = switch_on_off_timer_mode;

    /* 4.20 */
    struct test_case set_in_timer_mode = { "set in timer mode", test_led_set_in_timer_mode };
    test_led.cases[19] = set_in_timer_mode;

    /* 4.21 */
    struct test_case configure_on_off_mode = { "on/off mode", test_led_configure_on_off_mode };
    test_led.cases[20] = configure_on_off_mode;

    /* 4.22 */
    struct test_case switch_on_after_on_off_mode = { "switch on after setting in on/off mode", test_led_switch_on };
    test_led.cases[21] = switch_on_after_on_off_mode;

    /* 4.23 */
    test_led.cases[22] = release;

    ret = run_test(test_led);
    free(test_led.cases);

    return ret;
}

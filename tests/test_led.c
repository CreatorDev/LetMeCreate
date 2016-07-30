/**
 * @brief Implement LED section of miscellaneous/testing_plan
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "letmecreate/core/led.h"

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

static bool test_led_get_mode_invalid_index(void)
{
    uint8_t mode;
    return led_get_mode(LED_0 | LED_1, &mode) == -1;
}

static bool test_led_get_mode_null_var(void)
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
    int ret = -1;

    CREATE_TEST(led, 21)
    ADD_TEST_CASE(led, switch_on_off_before_init);
    ADD_TEST_CASE(led, set_before_init);
    ADD_TEST_CASE(led, set_delay_before_init);
    ADD_TEST_CASE(led, release_before_init);
    ADD_TEST_CASE(led, init);
    ADD_TEST_CASE(led, release);
    ADD_TEST_CASE(led, switch_on);
    ADD_TEST_CASE(led, switch_off);
    ADD_TEST_CASE(led, pattern_1);
    ADD_TEST_CASE(led, pattern_2);
    ADD_TEST_CASE(led, set_delay_on_off_mode);
    ADD_TEST_CASE(led, timer_mode);
    ADD_TEST_CASE(led, get_mode_invalid_index);
    ADD_TEST_CASE(led, get_mode_null_var);
    ADD_TEST_CASE(led, set_delay_slow);
    ADD_TEST_CASE(led, set_delay_fast);
    ADD_TEST_CASE(led, switch_on_off_timer_mode);
    ADD_TEST_CASE(led, set_in_timer_mode);
    ADD_TEST_CASE(led, configure_on_off_mode);
    ADD_TEST_CASE(led, switch_on);
    ADD_TEST_CASE(led, release);

    ret = run_test(test_led);
    free(test_led.cases);

    return ret;
}

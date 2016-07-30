/**
 * @brief Implement GPIO section of miscellaneous/testing_plan
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "letmecreate/core/gpio.h"

static bool test_gpio_get_set_direction_invalid_gpio(void)
{
    uint8_t direction;
    return gpio_get_direction(200, &direction) == -1
        && gpio_set_direction(200, GPIO_OUTPUT) == -1;
}

static bool test_gpio_get_set_value_invalid_gpio(void)
{
    uint8_t value;
    return gpio_get_value(200, &value) == -1
        && gpio_set_value(200, 1) == -1;
}

static bool test_gpio_init_invalid_gpio(void)
{
    return gpio_init(200) == -1;
}

static bool test_gpio_release_invalid_gpio(void)
{
    return gpio_release(200) == -1;
}

static bool test_gpio_init(void)
{
    uint8_t direction;

    if (gpio_init(MIKROBUS_1_INT) < 0)
        return -1;

    if (gpio_get_direction(MIKROBUS_1_INT, &direction) < 0)
        return -1;

    return direction == GPIO_INPUT;
}

static bool test_gpio_direction_input(void)
{
    uint8_t direction;
    if (gpio_set_direction(MIKROBUS_1_INT, GPIO_INPUT) < 0)
        return false;

    if(gpio_get_direction(MIKROBUS_1_INT, &direction) < 0)
        return false;

    return direction == GPIO_INPUT;
}

static bool test_gpio_set_value_invalid_direction(void)
{
    return gpio_set_value(MIKROBUS_1_INT, 1) == -1;
}

static bool test_gpio_input_gnd(void)
{
    uint8_t value;

    printf("Connect Mikrobus 1 INT gpio to ground.\n");
    printf("Press a switch when this is done.\n");
    if (wait_for_switch(30) < 0)
        return false;

    if (gpio_get_value(MIKROBUS_1_INT, &value) < 0)
        return false;

    return value == 0;
}

static bool test_gpio_input_3v3(void)
{
    uint8_t value;

    printf("Connect Mikrobus 1 INT gpio to 3V3.\n");
    printf("Press a switch when this is done.\n");
    if (wait_for_switch(30) < 0)
        return false;

    if (gpio_get_value(MIKROBUS_1_INT, &value) < 0)
        return false;

    return value == 1;
}

static bool test_gpio_direction_output(void)
{
    uint8_t direction;
    printf("Wire a LED to Mikrobus 1 INT gpio.\n");
    printf("Press a switch when this is done.\n");
    if (wait_for_switch(30) < 0)
        return false;

    if (gpio_set_direction(MIKROBUS_1_INT, GPIO_OUTPUT) < 0)
        return false;

    if (gpio_get_direction(MIKROBUS_1_INT, &direction) < 0)
        return false;

    return direction == GPIO_OUTPUT;
}

static bool test_gpio_output_high(void)
{
    uint8_t value;

    if (gpio_set_value(MIKROBUS_1_INT, 1) < 0)
        return false;
    if (gpio_get_value(MIKROBUS_1_INT, &value) < 0)
        return false;
    if (value != 1)
        return false;

    return ask_question("Is the LED on ?", 15) == 1;
}

static bool test_gpio_output_low(void)
{
    uint8_t value;

    if (gpio_set_value(MIKROBUS_1_INT, 0) < 0)
        return false;
    if (gpio_get_value(MIKROBUS_1_INT, &value) < 0)
        return false;
    if (value != 0)
        return false;

    return ask_question("Is the LED off ?", 15) == 1;
}

static bool test_gpio_release(void)
{
    return gpio_release(MIKROBUS_1_INT) == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(gpio, 13)
    ADD_TEST_CASE(gpio, get_set_direction_invalid_gpio);
    ADD_TEST_CASE(gpio, get_set_value_invalid_gpio);
    ADD_TEST_CASE(gpio, init_invalid_gpio);
    ADD_TEST_CASE(gpio, release_invalid_gpio);
    ADD_TEST_CASE(gpio, init);
    ADD_TEST_CASE(gpio, direction_input);
    ADD_TEST_CASE(gpio, set_value_invalid_direction);
    ADD_TEST_CASE(gpio, input_gnd);
    ADD_TEST_CASE(gpio, input_3v3);
    ADD_TEST_CASE(gpio, direction_output);
    ADD_TEST_CASE(gpio, output_high);
    ADD_TEST_CASE(gpio, output_low);
    ADD_TEST_CASE(gpio, release);

    ret = run_test(test_gpio);
    free(test_gpio.cases);

    return ret;
}

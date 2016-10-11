/**
 * @brief Implement GPIO MONITOR section of miscellaneous/testing_plan
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/gpio_monitor.h>

static int callback_ID;
static uint8_t gpio_event;

static void callback(uint8_t event_type)
{
    gpio_event = event_type;
}

static bool test_gpio_monitor_add_callback_before_init(void)
{
    return gpio_monitor_add_callback(MIKROBUS_1_INT, GPIO_EDGE, callback) == -1;
}

static bool test_gpio_monitor_init(void)
{
    return gpio_monitor_init() == 0
        && gpio_monitor_init() == 0;
}

static bool test_gpio_monitor_add_callback(void)
{
    unsigned int timeout;

    if (gpio_init(MIKROBUS_1_INT) < 0)
        return false;

    if ((callback_ID = gpio_monitor_add_callback(MIKROBUS_1_INT, GPIO_EDGE, callback)) < 0)
        return false;

    printf("Connect Mikrobus 1 INT gpio to GND.\n");
    gpio_event = 0;
    timeout = 30;
    while (gpio_event != GPIO_FALLING && timeout > 0) {
        sleep(1);
        --timeout;
    }

    if (timeout == 0) {
        printf("Timeout.\n");
        return false;
    }

    printf("Connect Mikrobus 1 INT gpio to 3V3.\n");
    gpio_event = 0;
    timeout = 30;
    while (gpio_event != GPIO_RAISING && timeout > 0) {
        sleep(1);
        --timeout;
    }

    if (timeout == 0) {
        printf("Timeout.\n");
        return false;
    }

    return true;
}

static bool test_gpio_monitor_remove_callback_invalid_id(void)
{
    return gpio_monitor_remove_callback(callback_ID+1) == -1;
}

static bool test_gpio_monitor_remove_callback(void)
{
    return gpio_monitor_remove_callback(callback_ID) == 0;

}

static bool test_gpio_monitor_remove_twice_callback(void)
{
    return gpio_monitor_remove_callback(callback_ID) == -1;
}

static bool test_gpio_monitor_release(void)
{
    return gpio_monitor_release() == 0
        && gpio_monitor_release() == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(gpio_monitor, 7)
    ADD_TEST_CASE(gpio_monitor, add_callback_before_init);
    ADD_TEST_CASE(gpio_monitor, init);
    ADD_TEST_CASE(gpio_monitor, add_callback);
    ADD_TEST_CASE(gpio_monitor, remove_callback_invalid_id);
    ADD_TEST_CASE(gpio_monitor, remove_callback);
    ADD_TEST_CASE(gpio_monitor, remove_twice_callback);
    ADD_TEST_CASE(gpio_monitor, release);

    ret = run_test(test_gpio_monitor);
    free(test_gpio_monitor.cases);

    return ret;
}

#include <stdbool.h>
#include <stdio.h>
#include <letmecreate/click/opto.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>

#define CHANNEL_COUNT       (4)

static const uint8_t channel_pins[MIKROBUS_COUNT][CHANNEL_COUNT] = {
{ MIKROBUS_1_INT, 0, MIKROBUS_1_RST, MIKROBUS_1_AN },
{ MIKROBUS_2_INT, 0, MIKROBUS_2_RST, MIKROBUS_2_AN }
};

static bool check_mikrobus_index(uint8_t mikrobus_index)
{
    if (mikrobus_index != MIKROBUS_1 && mikrobus_index != MIKROBUS_2) {
        fprintf(stderr, "opto: Invalid mikrobus index.\n");
        return false;
    }

    return true;
}

static bool check_channel_index(uint8_t channel_index)
{
    if (channel_index == OPTO_CHANNEL_2) {
        fprintf(stderr, "opto: Channel 2 is not supported on Ci40.\n");
        return false;
    }

    if (channel_index > OPTO_CHANNEL_4) {
        fprintf(stderr, "opto: Invalid channel index.\n");
        return false;
    }

    return true;
}

int opto_click_init(uint8_t mikrobus_index)
{
    int ret = 0;
    uint8_t i = 0;
    uint8_t gpio_pin;

    if (check_mikrobus_index(mikrobus_index) == false)
        return -1;

    gpio_pin = channel_pins[mikrobus_index][i];
    for (; i < CHANNEL_COUNT; ++i) {
        if (gpio_init(gpio_pin) < 0
        ||  gpio_set_direction(gpio_pin, GPIO_OUTPUT) < 0
        ||  gpio_set_value(gpio_pin, 0) < 0) {
            fprintf(stderr, "opto: Could not initialise channel %u\n", i);
            ret = -1;
        }
    }

    return ret;
}

int opto_click_enable(uint8_t mikrobus_index, uint8_t channel_index)
{
    if (check_mikrobus_index(mikrobus_index) == false
    ||  check_channel_index(channel_index) == false)
        return -1;

    return gpio_set_value(channel_pins[mikrobus_index][channel_index], 1);
}

int opto_click_disable(uint8_t mikrobus_index, uint8_t channel_index)
{
    if (check_mikrobus_index(mikrobus_index) == false
    ||  check_channel_index(channel_index) == false)
        return -1;

    return gpio_set_value(channel_pins[mikrobus_index][channel_index], 0);
}

int opto_click_release(uint8_t mikrobus_index)
{
    int ret = 0;
    uint8_t i = 0;

    if (check_mikrobus_index(mikrobus_index) == false)
        return -1;

    for (; i < CHANNEL_COUNT; ++i) {
        if (gpio_release(channel_pins[mikrobus_index][i]) < 0) {
            fprintf(stderr, "opto: Could not release channel %u\n", i);
            ret = -1;
        }
    }

    return ret;
}

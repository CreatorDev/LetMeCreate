#include <stdio.h>
#include "click/air_quality.h"
#include "core/common.h"
#include "core/gpio.h"
#include "core/gpio_monitor.h"

int air_quality_click_set_callback(uint8_t mikrobus_index, void(*callback)(uint8_t))
{
    uint8_t output_pin = 0;
    int callback_ID;

    if (callback == NULL) {
        fprintf(stderr, "air quality: Callback must not be null.\n");
        return -1;
    }

    switch (mikrobus_index) {
    case MIKROBUS_1:
        output_pin = MIKROBUS_1_AN;
        break;
    case MIKROBUS_2:
        output_pin = MIKROBUS_2_AN;
        break;
    default:
        fprintf(stderr, "air quality: Invalid mikrobus index.\n");
        return -1;
    }

    if (gpio_init(output_pin) < 0
    ||  gpio_set_direction(output_pin, GPIO_INPUT) < 0) {
        fprintf(stderr, "air quality: Failed to configure pin as an input.\n");
        return -1;
    }

    if (gpio_monitor_init() < 0)
        return -1;

    if ((callback_ID = gpio_monitor_add_callback(output_pin, GPIO_FALLING, callback)) < 0)
        fprintf(stderr, "air quality: Failed to add callback.\n");

    return callback_ID;
}

#include <stdio.h>
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"
#include "letmecreate/click/relay2.h"


int relay2_click_enable_relay_1(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_PWM)
        &&  !gpio_set_direction(MIKROBUS_1_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_PWM, 1))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_PWM)
        &&  !gpio_set_direction(MIKROBUS_2_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_PWM, 1))
            return 0;
        default:
            fprintf(stderr, "relay2: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay2_click_disable_relay_1(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_PWM)
        &&  !gpio_set_direction(MIKROBUS_1_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_PWM, 0))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_PWM)
        &&  !gpio_set_direction(MIKROBUS_2_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_PWM, 0))
            return 0;
        default:
            fprintf(stderr, "relay2: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay2_click_enable_relay_2(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_AN)
        &&  !gpio_set_direction(MIKROBUS_1_AN, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_AN, 1))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_AN)
        &&  !gpio_set_direction(MIKROBUS_2_AN, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_AN, 1))
            return 0;
        default:
            fprintf(stderr, "relay2: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay2_click_disable_relay_2(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_AN)
        &&  !gpio_set_direction(MIKROBUS_1_AN, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_AN, 0))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_AN)
        &&  !gpio_set_direction(MIKROBUS_2_AN, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_AN, 0))
            return 0;
        default:
            fprintf(stderr, "relay2: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

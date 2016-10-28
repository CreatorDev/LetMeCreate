#include <stdio.h>
#include <letmecreate/click/relay.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>


static uint8_t find_gpio(uint8_t mikrobus_index)
{
    uint8_t gpio_pin = 0;

    switch (mikrobus_index) {
    case MIKROBUS_1:
        gpio_pin = MIKROBUS_1_PWM;
        break;
    case MIKROBUS_2:
        gpio_pin = MIKROBUS_2_PWM;
        break;
    default:
        fprintf(stderr, "relay: Invalid mikrobus index.\n");
        break;
    }

    return gpio_pin;
}

int relay_click_enable_relay_1(uint8_t mikrobus_index)
{
    return relay_click_set_relay_1_state(mikrobus_index, 1);
}

int relay_click_disable_relay_1(uint8_t mikrobus_index)
{
    return relay_click_set_relay_1_state(mikrobus_index, 0);
}

int relay_click_set_relay_1_state(uint8_t mikrobus_index, uint8_t state)
{
    uint8_t gpio_pin = find_gpio(mikrobus_index);
    if (gpio_pin == 0)
        return -1;

    if (gpio_init(gpio_pin) < 0
    ||  gpio_set_direction(gpio_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(gpio_pin, !!state) < 0)
        return -1;

    return 0;
}

int relay_click_get_relay_1_state(uint8_t mikrobus_index, uint8_t *state)
{
    uint8_t gpio_pin = find_gpio(mikrobus_index);
    if (gpio_pin == 0)
        return -1;

    if (state == NULL) {
        fprintf(stderr, "relay: \n");
        return -1;
    }

    return gpio_get_value(gpio_pin, state);
}

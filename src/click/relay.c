#include <stdio.h>
#include <letmecreate/click/relay.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>


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
    uint8_t gpio_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_PWM, &gpio_pin) < 0
    ||  gpio_init(gpio_pin) < 0
    ||  gpio_set_direction(gpio_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(gpio_pin, !!state) < 0)
        return -1;

    return 0;
}

int relay_click_get_relay_1_state(uint8_t mikrobus_index, uint8_t *state)
{
    uint8_t gpio_pin;
    if (gpio_get_pin(mikrobus_index, TYPE_PWM, &gpio_pin) < 0)
        return -1;

    if (state == NULL) {
        fprintf(stderr, "relay: \n");
        return -1;
    }

    return gpio_get_value(gpio_pin, state);
}

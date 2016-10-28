#include <stdio.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/click/relay2.h>


static int find_gpio(uint8_t mikrobus_index, uint8_t relay)
{
    uint8_t gpio_pin = 0;

    if (mikrobus_index != MIKROBUS_1 && mikrobus_index != MIKROBUS_2) {
        fprintf(stderr, "relay2: Invalid mikrobus index.\n");
        return -1;
    }

    if (relay >= RELAY2_CLICK_RELAY_COUNT) {
        fprintf(stderr, "relay2: Invalid relay index.\n");
        return -1;
    }

    if (mikrobus_index == MIKROBUS_1) {
        if (relay == RELAY2_CLICK_RELAY_1)
            gpio_pin = MIKROBUS_1_PWM;
        else
            gpio_pin = MIKROBUS_1_AN;
    } else { /* mikrobus_index == MIKROBUS_2 */
        if (relay == RELAY2_CLICK_RELAY_1)
            gpio_pin = MIKROBUS_2_PWM;
        else
            gpio_pin = MIKROBUS_2_AN;
    }

    return gpio_pin;
}

int relay2_click_enable_relay(uint8_t mikrobus_index, uint8_t relay)
{
    return relay2_click_set_relay_state(mikrobus_index, relay, 1);
}

int relay2_click_disable_relay(uint8_t mikrobus_index, uint8_t relay)
{
    return relay2_click_set_relay_state(mikrobus_index, relay, 0);
}

int relay2_click_set_relay_state(uint8_t mikrobus_index, uint8_t relay, uint8_t state)
{
    uint8_t gpio_pin = 0;
    int ret = 0;

    ret = find_gpio(mikrobus_index, relay);
    if (ret < 0)
        return -1;

    gpio_pin = (uint8_t)ret;

    if (gpio_init(gpio_pin) < 0
    ||  gpio_set_direction(gpio_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(gpio_pin, !!state) < 0)
        return -1;

    return 0;
}

int LETMECREATE_CLICK_EXPORT relay2_click_get_relay_state(uint8_t mikrobus_index, uint8_t relay, uint8_t *state)
{
    uint8_t gpio_pin = 0;
    int ret = 0;

    if (state == NULL) {
        fprintf(stderr, "relay2: Cannot store state using null pointer.\n");
        return -1;
    }

    ret = find_gpio(mikrobus_index, relay);
    if (ret < 0)
        return -1;

    gpio_pin = (uint8_t)ret;

    return gpio_get_value(gpio_pin, state);
}

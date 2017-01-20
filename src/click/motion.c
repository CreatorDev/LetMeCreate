#include <stdio.h>
#include <letmecreate/click/motion.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio_monitor.h>


int motion_click_enable(uint8_t mikrobus_index)
{
    uint8_t gpio_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &gpio_pin) < 0)
        return -1;

    if (!gpio_init(gpio_pin)
    &&  !gpio_set_direction(gpio_pin, GPIO_OUTPUT)
    &&  !gpio_set_value(gpio_pin, 1))
        return 0;

    return -1;
}

int motion_click_attach_callback(uint8_t mikrobus_index, void(*callback)(uint8_t))
{
    uint8_t gpio_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_INT, &gpio_pin) < 0)
        return -1;

    if (!gpio_init(gpio_pin)
    &&  !gpio_set_direction(gpio_pin, GPIO_INPUT)
    &&  !gpio_monitor_init())
        return gpio_monitor_add_callback(gpio_pin, GPIO_EDGE, callback);

    return -1;
}

int motion_click_disable(uint8_t mikrobus_index)
{
    uint8_t gpio_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &gpio_pin) < 0)
        return -1;

    if (!gpio_init(gpio_pin)
    &&  !gpio_set_direction(gpio_pin, GPIO_OUTPUT)
    &&  !gpio_set_value(gpio_pin, 0))
        return 0;

    return -1;
}


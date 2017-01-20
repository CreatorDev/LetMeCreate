#include <stdio.h>
#include <letmecreate/click/ir_eclipse.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/gpio_monitor.h>


int ir_eclipse_click_add_callback(uint8_t mikrobus_index, void (*callback)(uint8_t))
{
    uint8_t gpio_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_INT, &gpio_pin) < 0) {
        fprintf(stderr, "ir_eclipse: Could not find pin\n");
        return -1;
    }

    if (callback == NULL) {
        fprintf(stderr, "ir_eclipse: Callback must not be null.\n");
        return -1;
    }

    if (gpio_init(gpio_pin) < 0
    ||  gpio_set_direction(gpio_pin, GPIO_INPUT) < 0
    ||  gpio_monitor_init() < 0)
        return -1;

    return gpio_monitor_add_callback(gpio_pin, GPIO_EDGE, callback);
}

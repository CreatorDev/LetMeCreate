#include <stdio.h>
#include <letmecreate/click/uni_hall.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/gpio_monitor.h>


int uni_hall_click_attach_callback(uint8_t mikrobus_index, void (*callback)(uint8_t))
{
    uint8_t gpio_pin = 0;

    if (callback == NULL) {
        fprintf(stderr, "uni_hall: Cannot attach null callback.\n");
        return -1;
    }

    if (gpio_get_pin(mikrobus_index, TYPE_INT, &gpio_pin) < 0
    ||  gpio_init(gpio_pin) < 0     /* It configures the gpio in input mode */
    ||  gpio_monitor_init() < 0)
        return -1;

    return gpio_monitor_add_callback(gpio_pin, GPIO_EDGE, callback);
}

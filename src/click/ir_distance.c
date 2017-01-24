#include <stdbool.h>
#include <stdio.h>
#include <letmecreate/click/ir_distance.h>
#include <letmecreate/core/adc.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>


static int set_enable_pin(uint8_t mikrobus_index, bool enable)
{
    uint8_t rst_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &rst_pin) < 0) {
        fprintf(stderr, "ir_distance: Failed to find pin\n");
        return -1;
    }

    if (gpio_init(rst_pin) < 0
    ||  gpio_set_direction(rst_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(rst_pin, enable ? 1 : 0) < 0) {
        if (enable)
            fprintf(stderr, "ir_distance: Failed to enable chip.\n");
        else
            fprintf(stderr, "ir_distance: Failed to disable chip.\n");
        return -1;
    }

    return 0;
}

int ir_distance_click_enable(uint8_t mikrobus_index)
{
    return set_enable_pin(mikrobus_index, true);
}

int ir_distance_click_get_measure(uint8_t mikrobus_index, uint16_t *measure)
{
    float voltage = 0;

    if (measure == NULL) {
        fprintf(stderr, "ir_distance: Cannot store measure to null variable.\n");
        return -1;
    }

    if (adc_get_value(mikrobus_index, &voltage) < 0)
        return -1;

    *measure = (voltage / 5.f) * 65535;

    return 0;
}

int ir_distance_click_disable(uint8_t mikrobus_index)
{
    return set_enable_pin(mikrobus_index, false);
}

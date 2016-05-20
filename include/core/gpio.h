/**
 * @file gpio.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_GPIO_H__
#define __LETMECREATE_CORE_GPIO_H__

#include <stdint.h>

enum GPIO_PIN {
    MIKROBUS_1_AN,
    MIKROBUS_1_RST,
    MIKROBUS_1_INT,
    MIKROBUS_2_AN,
    MIKROBUS_2_RST,
    MIKROBUS_2_INT,
    GPIO_CNT
};

enum GPIO_DIR {
    GPIO_OUTPUT,
    GPIO_INPUT
};

int gpio_init(const uint8_t gpio_pin);
int gpio_set_direction(const uint8_t gpio_pin, const uint8_t dir);
int gpio_get_direction(const uint8_t gpio_pin, uint8_t *dir);
int gpio_set_value(const uint8_t gpio_pin, const uint8_t value);
int gpio_get_value(const uint8_t gpio_pin, uint8_t *value);
int gpio_release(const uint8_t gpio_pin);

#endif

/**
 * @file gpio_monitor.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_GPIO_MONITOR_H__
#define __LETMECREATE_CORE_GPIO_MONITOR_H__

#include <stdint.h>
#include "gpio.h"

/** Event on GPIO */
enum GPIO_EVENT {
    GPIO_RAISING   = 0x01,
    GPIO_FALLING   = 0x02,
    GPIO_EDGE      = 0x03
};


int gpio_monitor_init(void);
int gpio_monitor_add_callback(const uint8_t gpio_pin, const uint8_t event_mask, void(*callback)(uint8_t));
int gpio_monitor_remove_callback(const int callback_ID);
void gpio_monitor_release(void);

#endif

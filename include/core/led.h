#ifndef __LETMECREATE_CORE_LED_H__
#define __LETMECREATE_CORE_LED_H__

#include <stdint.h>

typedef enum LED_INDEX {
    LED_0 = 0x01,
    LED_1 = 0x02,
    LED_2 = 0x04,
    LED_3 = 0x08,
    LED_4 = 0x10,
    LED_5 = 0x20,
    LED_6 = 0x40
} LED_INDEX;

int led_initialize(void);
int led_switch_on(const uint8_t led_mask);
int led_switch_off(const uint8_t led_mask);
int led_set(const uint8_t led_mask, const uint8_t led_value);
void led_release(void);

#endif

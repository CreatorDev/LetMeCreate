/**
 * @file led.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

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

/**
 * @brief Initialise file descriptors for each LED. Switch off all LED'S.
 * @return 0 if successful, -1 otherwise
 */
int led_init(void);

/**
 * @brief Switch on some LEDS.
 *
 * @param[in] led_mask
 * @return 0 if successful, -1 otherwise
 */
int led_switch_on(const uint8_t led_mask);

/**
 * @brief Switch off some LED'S.
 *
 * @param[in] led_mask
 * @return 0 if successful, -1 otherwise
 */
int led_switch_off(const uint8_t led_mask);

/**
 * @brief Switch on/off each LED depending on corresponding bit in @p led_value
 *
 * @param[in] led_mask
 * @param[in] led_value
 * @return 0 if successful, -1 otherwise
 */
int led_set(const uint8_t led_mask, const uint8_t led_value);

/**
 * @brief Close file descriptors for each LED.
 */
void led_release(void);

#endif

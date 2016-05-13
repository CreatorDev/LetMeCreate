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
    LED_6 = 0x40,
    LED_HEARTBEAT=0x80
} LED_INDEX;

/**
 * @brief Initialise file descriptors for each LED. Switch off all LEDs.
 * @return 0 if successful, -1 otherwise
 */
int led_init(void);

/**
 * @brief Switch on some LEDs.
 *
 * @param[in] mask
 * @return 0 if successful, -1 otherwise
 */
int led_switch_on(const uint8_t mask);

/**
 * @brief Switch off some LEDs.
 *
 * @param[in] mask
 * @return 0 if successful, -1 otherwise
 */
int led_switch_off(const uint8_t mask);

/**
 * @brief Switch on/off each LED depending on corresponding bit in @p led_value
 *
 * Each bit of the mask represents a LED. Only LED's whose corresponding bit in mask is set to 1
 * will be switched on or off by this function depending on corresponding bit .
 *
 * For instance, to turn on LED_2 you have to set bit 2 of mask to 1 and bit 2 of value to 1. So,
 * mask  = LED_2
 * value = 0x04
 *
 * @param[in] mask
 * @param[in] value
 * @return 0 if successful, -1 otherwise
 */
int led_set(const uint8_t mask, const uint8_t value);

/**
 * @brief Close file descriptors for each LED.
 */
void led_release(void);

#endif

/**
 * @file attiny88.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_ATTINY88_H__
#define __LETMECREATE_RPISENSEHAT_ATTINY88_H__

#include <stdint.h>

/** Values for each joystick position */
enum RPISENSEHAT_JOYSTICK_STATE {
    JOYSTICK_LEFT       = 0x10,
    JOYSTICK_RIGHT      = 0x02,
    JOYSTICK_DOWN       = 0x01,
    JOYSTICK_UP         = 0x04,
    JOYSTICK_PRESSED    = 0x08
};

/**
 * @brief Enable the Atmel Chip.
 *
 * Reset the chip and switch off all LED's.
 *
 * @return 0 if successful, -1 otherwise
 */
int attiny88_enable(void);

/**
 * @brief Retrieve joystick position.
 *
 * @param[out] state Position of the joystick (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int attiny88_get_joystick_input(uint8_t *state);

/**
 * @brief Set LED's matrix
 *
 * The pixel buffer must be in this format:
 *
 * first row:       RGB RGB RGB RGB RGB RGB RGB RGB
 * second row:      RGB RGB RGB RGB RGB RGB RGB RGB
 * ...
 *
 * Each value of the array must be not be greater than 0x1F.
 *
 * @param[in] pixels 192 bytes long array
 * @return 0 if successful, -1 otherwise
 */
int attiny88_set_leds_state(uint8_t *pixels);

/**
 * @brief Display a rainbow on the LED's matrix.
 *
 * @return 0 if successful, -1 otherwise
 */
int attiny88_display_rainbow(void);

/**
 * @brief Swich off all LED's.
 *
 * @return 0 if successful, -1 otherwise
 */
int attiny88_disable(void);

#endif

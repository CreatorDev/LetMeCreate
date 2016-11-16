/**
 * @file attiny88.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_ATTINY88_H__
#define __LETMECREATE_RPISENSEHAT_ATTINY88_H__

#include <stdint.h>

enum RPISENSEHAT_JOYSTICK_STATE {
    JOYSTICK_LEFT       = 0x10,
    JOYSTICK_RIGHT      = 0x02,
    JOYSTICK_DOWN       = 0x01,
    JOYSTICK_UP         = 0x04,
    JOYSTICK_PRESSED    = 0x08
};

int attiny88_enable(void);
int attiny88_get_joystick_input(uint8_t *state);
int attiny88_set_leds_state(uint8_t *pixels);
int attiny88_display_rainbow(void);
int attiny88_disable(void);
#endif
